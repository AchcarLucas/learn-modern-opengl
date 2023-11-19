#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "framebuffer.hpp"
#include "ubo.hpp"
#include "object.hpp"

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

static std::vector<Vertex> ex_30_quad_vertices = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<GLuint> ex_30_quad_indices = {
    3, 1, 0,
    3, 2, 1
};

int run_031(const int width, const int height)
{
    float _WIDTH = width;
    float _HEIGHT = height;

    _stbi_set_flip_vertically_on_load(true);

    camera->setCamSpeed(25.0f);

    Mesh *mesh_screen = new Mesh(ex_30_quad_vertices, ex_30_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);

    UBO *ubo_matrices = new UBO("Matrices", 2 * sizeof(glm::mat4), 0);
    UBO *ubo_camera = new UBO("Camera", sizeof(glm::vec3), 1);

    {
        glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camera->getCamPos(), camera->getCamPos() + camera->getCamFront(), camera->getUpVector());

        ubo_matrices->UBOSubBuffer(glm::value_ptr(projection), 0, sizeof(glm::mat4));
        ubo_matrices->UBOSubBuffer(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));

        ubo_camera->UBOSubBuffer(glm::value_ptr(camera->getCamPos()), 0, sizeof(glm::vec3));
    }

    Shader *shader_screen = new Shader("glsl/ex_31/posprocessing.vs", "glsl/ex_31/posprocessing.fs");
    Shader *shader_floor = new Shader("glsl/ex_31/floor.vs", "glsl/ex_31/floor.fs");
    Shader *shader_light = new Shader("glsl/ex_31/light.vs", "glsl/ex_31/light.fs");

    shader_floor->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
    shader_floor->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());
    shader_light->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());

    shader_screen->use();
    shader_screen->setFloat("gama", 2.2f);

    Texture2D *texture_wood = new Texture2D("./resources/textures/wood.png", TextureType::DIFFUSE);

    std::vector<Texture2D*> floor_textures = {
        texture_wood
    };

    Mesh *floor = new Mesh(ex_30_quad_vertices, ex_30_quad_indices, floor_textures, VERTEX_TYPE::ATTRIB_PNT);

    SObject *light = createCube();

    // msaa com multisample 4
    FrameBuffer *msaa_buffer = new FrameBuffer(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, 4);
    // buffer de tela
    FrameBuffer *screen_buffer = new FrameBuffer(width, height);

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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // global matrices
        {
            glm::mat4 view = glm::lookAt(camera->getCamPos(), camera->getCamPos() + camera->getCamFront(), camera->getUpVector());
            ubo_matrices->UBOSubBuffer(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));
        }

        // global camera
        {
            ubo_camera->UBOSubBuffer(glm::value_ptr(camera->getCamPos()), 0, sizeof(glm::vec3));
        }

        //glm::vec4 light_position = glm::vec4(camera->getCamPos() + (camera->getCamFront() * 3.0f), 0.0f);
        glm::vec4 light_position = glm::vec4(0.0f, 0.0f, -3.0f, 0.0f);
        bool light_enabled = true;

        // global lights
        {
            shader_floor->use();
            shader_floor->setUniform4fv("lights[0].position", glm::value_ptr(light_position));
            shader_floor->setUniform4fv("lights[0].ambient", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 0.05f));
            shader_floor->setUniform4fv("lights[0].diffuse", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
            shader_floor->setUniform4fv("lights[0].specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

            shader_floor->setUniform3fv("lights[0].spot_direction", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

            shader_floor->setFloat("lights[0].constant_attenuation", 0.0f);
            shader_floor->setFloat("lights[0].linear_attenuation", 0.0f);
            shader_floor->setFloat("lights[0].quadratic_attenuation", 0.0f);

            shader_floor->setFloat("lights[0].spot_cutoff", 0.0f);
            shader_floor->setFloat("lights[0].spot_exponent", 0.0f);

            shader_floor->setBool("lights[0].enabled", light_enabled);

        }

        // draw lights
        {
            if(light_enabled) {
                light->_vao->bind();
                light->_ebo->bind();

                glm::mat4 model(1.0f);

                model = glm::translate(model, glm::vec3(light_position));
                model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

                shader_light->use();
                shader_light->setMatrix4fv("model", glm::value_ptr(model));

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

        }

        // draw object
        {
            glm::mat4 model = glm::mat4(1.0f);


            model = glm::translate(model, glm::vec3(0.0f, -1.0f, -1.0f));
            model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            shader_floor->use();
            shader_floor->setMatrix4fv("model", glm::value_ptr(model));

            floor->draw(shader_floor);
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
            screen_buffer->getTexture2D()->bind(GL_TEXTURE0);
            mesh_screen->draw(shader_screen);
        }

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader_floor;
    delete shader_screen;

    delete mesh_screen;
    delete floor;
    delete camera;

    delete texture_wood;

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

