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

static void loadScene(GLFWwindow*, const int, const int);

static void updateShader(GLFWwindow*, const int, const int);

static void renderScene(GLFWwindow*, const int, const int);
static void renderLight(GLFWwindow*, const int, const int);
static void renderProcessing(GLFWwindow*, const int, const int);

// camera class
static Camera *camera = new Camera();

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

static std::vector<Vertex> ex_32_quad_vertices_screen = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_32_quad_vertices_floor = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(10.0f, 10.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(10.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 10.0f))
};

static std::vector<GLuint> ex_32_quad_indices = {
    3, 1, 0,
    3, 2, 1
};

static std::vector<Vertex> ex_32_cube_vertices = {
    // Back face
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    // Front face
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
    // Left face
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
    // Right face
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
    // Bottom face
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    // Top face
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, 0.5f),  glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f)),
};

static std::vector<GLuint> ex_32_cube_indices = {
    0, 1, 2,
    5, 4, 3,

    6, 7, 8,
    9, 10, 11,

    12, 13, 14,
    15, 16, 17,

    18, 19, 20,
    21, 22, 23,

    24, 25, 26,
    27, 28, 29,

    30, 31, 32,
    33, 34, 35
};

static Texture2D *texture_wood;
static Texture2D *texture_cube;

static Shader *shader_screen;
static Shader *shader_floor;
static Shader *shader_cube;
static Shader *shader_light;

// msaa com multisample 4
static FrameBuffer *msaa_buffer;
// buffer de tela
static FrameBuffer *screen_buffer;
// buffer shadow mapping
static FrameBuffer *shadow_buffer;

static UBO *ubo_matrices;
static UBO *ubo_camera;

static Mesh *mesh_screen;
static Mesh *mesh_floor;
static Mesh *mesh_cube;

static SObject *light;

static std::vector<Shader*> config_shader;

static std::vector<glm::vec3> cube_positions = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 0.0f,  1.5f,  1.5f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3( 2.0f,  5.0f, -10.0f),
};

static glm::vec4 light_position = glm::vec4(0.0f, 10.0f, 10.0f, 0.0f);

static bool light_enabled = true;

int run_032(const int width, const int height)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadScene(window, width, height);

    while(!glfwWindowShouldClose(window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        msaa_buffer->bind();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);


        updateShader(window, width, height);

        renderScene(window, width, height);
        renderLight(window, width, height);
        renderProcessing(window, width, height);

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader_floor;
    delete shader_screen;
    delete shader_cube;
    delete shader_light;

    delete mesh_screen;
    delete mesh_cube;
    delete mesh_floor;

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

static void loadScene(GLFWwindow* window, const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    camera->setCamSpeed(25.0f);

    shader_screen = new Shader("glsl/ex_32/posprocessing.vs", "glsl/ex_32/posprocessing.fs");
    shader_floor = new Shader("glsl/ex_32/floor.vs", "glsl/ex_32/floor.fs");
    shader_cube = new Shader("glsl/ex_32/cube.vs", "glsl/ex_32/cube.fs");
    shader_light = new Shader("glsl/ex_32/light.vs", "glsl/ex_32/light.fs");

    config_shader.push_back(shader_cube);
    config_shader.push_back(shader_floor);

    texture_wood = new Texture2D("./resources/textures/wood.png", TextureType::DIFFUSE, true, GL_SRGB);
    texture_cube = new Texture2D("./resources/textures/container2.png", TextureType::DIFFUSE, true, GL_SRGB);

    // msaa com multisample 4
    msaa_buffer = new FrameBuffer(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, 4);
    // buffer de tela
    screen_buffer = new FrameBuffer(width, height);
    // buffer shadow mapping
    shadow_buffer = new FrameBuffer(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);

    std::vector<Texture2D*> floor_textures = {
        texture_wood
    };

    std::vector<Texture2D*> cube_textures {
        texture_cube
    };

    mesh_screen = new Mesh(ex_32_quad_vertices_screen, ex_32_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);
    mesh_floor = new Mesh(ex_32_quad_vertices_floor, ex_32_quad_indices, floor_textures, VERTEX_TYPE::ATTRIB_PNT);
    mesh_cube = new Mesh(ex_32_cube_vertices, ex_32_cube_indices, cube_textures, VERTEX_TYPE::ATTRIB_PNT);

    ubo_matrices = new UBO("Matrices", 2 * sizeof(glm::mat4), 0);
    ubo_camera = new UBO("Camera", sizeof(glm::vec3), 1);

    {
        glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)width / (float)height, 0.1f, 1000.0f);
        glm::mat4 view = glm::lookAt(camera->getCamPos(), camera->getCamPos() + camera->getCamFront(), camera->getUpVector());

        ubo_matrices->UBOSubBuffer(glm::value_ptr(projection), 0, sizeof(glm::mat4));
        ubo_matrices->UBOSubBuffer(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));

        ubo_camera->UBOSubBuffer(glm::value_ptr(camera->getCamPos()), 0, sizeof(glm::vec3));
    }

    {
        shader_floor->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_floor->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());

        shader_cube->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_cube->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());

        shader_light->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
    }

    {
        shader_screen->use();
        shader_cube->setFloat("gama", 2.2f);
        shader_screen->setInt("screen_texture", 0);


        shader_cube->use();
        shader_cube->setFloat("gama", 2.2f);

        shader_floor->use();
        shader_floor->setFloat("gama", 2.2f);
    }

    light = createCube();
}

static void updateShader(GLFWwindow* window, const int width, const int height)
{
    // global matrices
    {
        glm::mat4 view = glm::lookAt(camera->getCamPos(), camera->getCamPos() + camera->getCamFront(), camera->getUpVector());
        ubo_matrices->UBOSubBuffer(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));
    }

    // global camera
    {
        ubo_camera->UBOSubBuffer(glm::value_ptr(camera->getCamPos()), 0, sizeof(glm::vec3));
    }

    // glm::vec4 light_position = glm::vec4(camera->getCamPos() + (camera->getCamFront() * 3.0f), 0.0f);


    light_position = light_position * glm::rotate(glm::mat4(1.0f), glm::radians(delta_time) * 100.0f, glm::vec3(0.0f, 1.0f, 0.0f));


    /// Global lights
    {
        for(auto *shader : config_shader) {
            shader->use();

            shader->setUniform4fv("lights[0].position", glm::value_ptr(light_position));
            shader->setUniform4fv("lights[0].ambient", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 0.05f));
            shader->setUniform4fv("lights[0].diffuse", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
            shader->setUniform4fv("lights[0].specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

            shader->setUniform3fv("lights[0].spot_direction", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

            shader->setFloat("lights[0].constant_attenuation", 0.0f);
            shader->setFloat("lights[0].linear_attenuation", 0.0f);
            shader->setFloat("lights[0].quadratic_attenuation", 0.0f);

            shader->setFloat("lights[0].spot_cutoff", 0.0f);
            shader->setFloat("lights[0].spot_exponent", 0.0f);

            shader->setBool("lights[0].enabled", light_enabled);
        }

    }
}

static void renderScene(GLFWwindow* window, const int width, const int height)
{
    /// Draw floor
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.51f, -1.0f));
        model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        shader_floor->use();
        shader_floor->setMatrix4fv("model", glm::value_ptr(model));

        mesh_floor->draw(shader_floor);
    }

    /// Draw Cube
    {
        for(unsigned int i = 0; i < cube_positions.size(); ++i) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cube_positions[i]);
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

            shader_cube->use();
            shader_cube->setMatrix4fv("model", glm::value_ptr(model));

            mesh_cube->draw(shader_cube);
        }
    }
}

static void renderLight(GLFWwindow* window, const int width, const int height)
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

static void renderProcessing(GLFWwindow* window, const int width, const int height)
{
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
}

