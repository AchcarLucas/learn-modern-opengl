#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "mesh.hpp"
#include "framebuffer.hpp"
#include "ubo.hpp"
#include "object.hpp"
#include "light.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static void clearGL();

static void loadScene(GLFWwindow*, const int, const int);

static void updateScene(GLFWwindow*, const int, const int);
static void updateShader(GLFWwindow*, const int, const int);

static void renderScene(GLFWwindow*, const int, const int, Shader *shader = nullptr);
static void renderLight(GLFWwindow*, const int, const int);
static void renderProcessing(GLFWwindow*, const int, const int);
static void renderProcessingDebug(GLFWwindow*, const int, const int);

static void processInput(GLFWwindow *, float);
static void mouseCallback(GLFWwindow*, double, double);
static void scrollCallback(GLFWwindow*, double, double);

// camera class
static Camera *camera = new Camera(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

static std::vector<Vertex> ex_32_quad_vertices_screen = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_32_quad_vertices_screen_debug = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.5f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
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
static Shader *shader_shadow;
static Shader *shader_debug;

// msaa com multisample 4
static FrameBuffer<Texture2D> *msaa_buffer;
// buffer de tela
static FrameBuffer<Texture2D> *screen_buffer;
// buffer shadow mapping
static FrameBuffer<Texture2D> *shadow_buffer;

static UBO *ubo_matrices;
static UBO *ubo_camera;

static Mesh *mesh_screen;
static Mesh *mesh_screen_debug;
static Mesh *mesh_floor;
static Mesh *mesh_cube;

static DirectionalLight *dir_light;
static SObject *obj_light;

static std::vector<Shader*> config_shader;

static std::vector<ModelTransform> cube_models = {
    ModelTransform(glm::vec3( 0.0f, 1.5f, 0.0), glm::vec3(1.0f)),
    ModelTransform(glm::vec3( 2.0f, 0.0f, 1.0), glm::vec3(1.0f)),
    ModelTransform(glm::vec3( -1.0f, 0.0f, 2.0), glm::vec3(0.5), glm::vec3(1.0, 0.0, 1.0), 60.0f)
};

static glm::vec4 light_position = glm::vec4(3.0f, 3.0f, 3.0f, 0.0f);

static bool light_enabled = true;
static float _gamma = 2.2f;

int run_033(const int width, const int height)
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

        updateScene(window, width, height);
        updateShader(window, width, height);

        glViewport(0, 0, shadow_buffer->getWidth(), shadow_buffer->getHeight());
        shadow_buffer->bind();
        clearGL();
        glCullFace(GL_FRONT);
        renderScene(window, width, height, shader_shadow);
        glCullFace(GL_BACK);

        glViewport(0, 0, screen_buffer->getWidth(), screen_buffer->getHeight());
        msaa_buffer->bind();
        clearGL();
        renderScene(window, width, height);
        renderLight(window, width, height);

        renderProcessing(window, width, height);
        renderProcessingDebug(window, width, height);

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

static void clearGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

static void loadScene(GLFWwindow* window, const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    camera->setCamSpeed(25.0f);

    dir_light = new DirectionalLight(light_position, glm::vec3(0.0, -2.0, 0.0), 4.0f, 1.0f, 20.0f);

    shader_screen = new Shader("glsl/ex_33/posprocessing.vs", "glsl/ex_33/posprocessing.fs");
    shader_floor = new Shader("glsl/ex_33/object.vs", "glsl/ex_33/object.fs");
    shader_cube = new Shader("glsl/ex_33/object.vs", "glsl/ex_33/object.fs");
    shader_light = new Shader("glsl/ex_33/light.vs", "glsl/ex_33/light.fs");
    shader_shadow = new Shader("glsl/ex_33/depth.vs", "glsl/ex_33/depth.fs");
    shader_debug = new Shader("glsl/ex_33/debug_depth.vs", "glsl/ex_33/debug_depth.fs");

    config_shader.push_back(shader_cube);
    config_shader.push_back(shader_floor);

    texture_wood = new Texture2D("./resources/textures/wood.png", TextureType::DIFFUSE, true, GL_SRGB);
    texture_cube = new Texture2D("./resources/textures/container2.png", TextureType::DIFFUSE, true, GL_SRGB);

    // msaa com multisample 4
    msaa_buffer = new FrameBuffer<Texture2D>(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, 4);
    // buffer de tela
    screen_buffer = new FrameBuffer<Texture2D>(width, height);
    // buffer shadow mapping
    shadow_buffer = new FrameBuffer<Texture2D>(2048, 2048, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, TextureType::FRAMEBUFFER_DEPTH_MAPPING);

    std::vector<Texture2D*> floor_textures = {
        texture_wood,
        shadow_buffer->getTexture()
    };

    std::vector<Texture2D*> cube_textures {
        texture_cube,
        shadow_buffer->getTexture()
    };

    mesh_screen = new Mesh(ex_32_quad_vertices_screen, ex_32_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);
    mesh_screen_debug = new Mesh(ex_32_quad_vertices_screen_debug, ex_32_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);
    mesh_floor = new Mesh(ex_32_quad_vertices_floor, ex_32_quad_indices, floor_textures, VERTEX_TYPE::ATTRIB_PNT);
    mesh_cube = new Mesh(ex_32_cube_vertices, ex_32_cube_indices, cube_textures, VERTEX_TYPE::ATTRIB_PNT);

    ubo_matrices = new UBO("Matrices", 2 * sizeof(glm::mat4), 0);
    ubo_camera = new UBO("Camera", sizeof(glm::vec3), 2);

    /// global matrices
    {
        glm::mat4 _projection = camera->getPerspectiveMatrix(width, height, 0.1f, 1000.0f);
        glm::mat4 _view = camera->getViewMatrix();

        ubo_matrices->UBOSubBuffer(glm::value_ptr(_projection), 0, sizeof(glm::mat4));
        ubo_matrices->UBOSubBuffer(glm::value_ptr(_view), sizeof(glm::mat4), sizeof(glm::mat4));
    }

    /// global camera
    {
        ubo_camera->UBOSubBuffer(glm::value_ptr(camera->getCamPos()), 0, sizeof(glm::vec3));
    }

    /// ubo attribute
    {
        shader_floor->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_floor->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());

        shader_cube->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_cube->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());

        shader_light->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
    }

    obj_light = createCube();
}

static void updateScene(GLFWwindow* window, const int width, const int height)
{
    // glm::vec4 light_position = glm::vec4(camera->getCamPos() + (camera->getCamFront() * 3.0f), 0.0f);
    light_position = light_position * glm::rotate(glm::mat4(1.0f), glm::radians(delta_time) * 100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    dir_light->setPosition(light_position);

    // global matrices
    {
        ubo_matrices->UBOSubBuffer(glm::value_ptr(camera->getViewMatrix()), sizeof(glm::mat4), sizeof(glm::mat4));
    }

    // global camera
    {
        ubo_camera->UBOSubBuffer(glm::value_ptr(camera->getCamPos()), 0, sizeof(glm::vec3));
    }
}

static void updateShader(GLFWwindow* window, const int width, const int height)
{
    static bool first = true;

    if(first) {
        shader_screen->use();
        shader_cube->setFloat("gamma", _gamma);
        shader_screen->setInt("screen_texture", 0);

        shader_cube->use();
        shader_cube->setFloat("gamma", _gamma);

        shader_floor->use();
        shader_floor->setFloat("gamma", _gamma);

        shader_debug->use();
        shader_debug->setInt("depth_map", 0);


        first = false;
    }

    /// Global lights
    {
        for(auto *shader : config_shader) {
            shader->use();

            shader->setUniform4fv("lights[0].position", glm::value_ptr(light_position));
            shader->setUniform4fv("lights[0].ambient", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));
            shader->setUniform4fv("lights[0].diffuse", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));
            shader->setUniform4fv("lights[0].specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));

            shader->setUniform3fv("lights[0].spot_direction", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

            shader->setFloat("lights[0].constant_attenuation", 0.0f);
            shader->setFloat("lights[0].linear_attenuation", 0.0f);
            shader->setFloat("lights[0].quadratic_attenuation", 0.0f);

            shader->setFloat("lights[0].spot_cutoff", 0.0f);
            shader->setFloat("lights[0].spot_exponent", 0.0f);

            shader->setFloat("lights[0].near_plane", dir_light->getNearPlane());
            shader->setFloat("lights[0].far_plane", dir_light->getFarPlane());

            shader->setBool("lights[0].enabled", light_enabled);
        }
    }

    {
        dir_light->settingShader(shader_shadow);
        dir_light->settingShader(shader_debug);
        dir_light->settingShader(shader_cube);
        dir_light->settingShader(shader_floor);
    }
}

static void renderScene(GLFWwindow* window, const int width, const int height, Shader *_shader)
{
    /// Draw floor
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.51f, -1.0f));
        model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        Shader *used = _shader ? _shader : shader_floor;

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_floor->draw(used);
    }

    /// Draw Cube
    {
        for(auto &model : cube_models) {
            Shader *used = _shader ? _shader : shader_cube;

            used->use();
            used->setMatrix4fv("model", glm::value_ptr(model.getModel()));

            mesh_cube->draw(used);
        }
    }
}

static void renderLight(GLFWwindow* window, const int width, const int height)
{
    if(light_enabled) {
        obj_light->_vao->bind();
        obj_light->_ebo->bind();

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
    glBlitFramebuffer(0, 0, msaa_buffer->getWidth(), msaa_buffer->getHeight(), 0, 0, screen_buffer->getWidth(), screen_buffer->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

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
}

static void renderProcessingDebug(GLFWwindow* window, const int width, const int height)
{
    msaa_buffer->unbind();

    shader_debug->use();
    shadow_buffer->getTexture()->bind(GL_TEXTURE0);
    mesh_screen_debug->draw(shader_debug);
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
