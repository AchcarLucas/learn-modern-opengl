#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "framebuffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static void processInput(GLFWwindow *, float);
static void mouseCallback(GLFWwindow*, double, double);
static void scrollCallback(GLFWwindow*, double, double);

// camera class
static Camera *camera = new Camera();

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

static std::vector<Vertex> ex_29_quad_vertices = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<GLuint> ex_29_quad_indices = {
    3, 1, 0,
    3, 2, 1
};

int run_029(const int width, const int height)
{
    float _WIDTH = width;
    float _HEIGHT = height;

    _stbi_set_flip_vertically_on_load(true);

    Mesh *mesh_screen = new Mesh(ex_29_quad_vertices, ex_29_quad_indices, std::vector<Texture2D*>());

    Shader *shader = new Shader("glsl/ex_29/green_cube.vs", "glsl/ex_29/green_cube.fs");
    Shader *shader_screen = new Shader("glsl/ex_29/anti_aliasing.vs", "glsl/ex_29/anti_aliasing.fs");

    SObject *cube = createCube();

    std::vector<glm::vec3> cube_positions = {
        glm::vec3(0.0f, 0.0f, 0.0f)
    };

    // msaa com multisample 4
    FrameBuffer<Texture2D> *msaa_buffer = new FrameBuffer<Texture2D>(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, 4);
    // buffer de tela
    FrameBuffer<Texture2D> *screen_buffer = new FrameBuffer<Texture2D>(width, height);

    shader_screen->use();
    shader_screen->setInt("screen_texture", 0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        msaa_buffer->bind();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camera->getCamPos(), camera->getCamPos() + camera->getCamFront(), camera->getUpVector());

        // draw object
        {
            shader->use();

            shader->setMatrix4fv("view", glm::value_ptr(view));
            shader->setMatrix4fv("projection", glm::value_ptr(projection));

            cube->_vao->bind();
            cube->_vbo->bind();
            cube->_ebo->bind();

            for(unsigned int i = 0; i < cube_positions.size(); i++) {
                glm::mat4 model = glm::mat4(1.0f);
                shader->setMatrix4fv("model", glm::value_ptr(model));
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }

        // copia o buffer da msaa para o buffer da tela
        msaa_buffer->bind(GL_READ_FRAMEBUFFER);
        screen_buffer->bind(GL_DRAW_FRAMEBUFFER);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        msaa_buffer->unbind();

        // draw screen
        {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);

            shader_screen->use();
            screen_buffer->getTexture()->bind(GL_TEXTURE0);
            mesh_screen->draw(shader_screen);
        }

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader;
    delete shader_screen;

    delete mesh_screen;
    delete cube;
    delete camera;

    glfwTerminate();
    return 0;
}

static void processInput(GLFWwindow *window, float delta_time)
{
    camera->processInput(window, delta_time);
}

static void mouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    camera->mouseCallback(window, x_pos, y_pos);
}

static void scrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    camera->scrollCallback(window, x_offset, y_offset);
}

