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
#include "rendertext.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static void clearGL();

static void loadScene(GLFWwindow*, const int, const int);

static void updateScene(GLFWwindow*, const int, const int);
static void updateShader(GLFWwindow*, const int, const int);

static void renderScene(GLFWwindow*, const int, const int, Shader *shader = nullptr);
static void renderProcessing(GLFWwindow*, const int, const int);
static void renderProcessingDebug(GLFWwindow*, const int, const int);

static void processInput(GLFWwindow *, float);
static void mouseCallback(GLFWwindow*, double, double);
static void scrollCallback(GLFWwindow*, double, double);

// camera class
static Camera *camera = new Camera(glm::vec3(15.0f, 15.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

static std::vector<Vertex> ex_44_quad_vertices_screen = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_44_quad_vertices_plane = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_44_quad_vertices_screen_debug = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.5f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<GLuint> ex_44_quad_indices = {
    3, 1, 0,
    3, 2, 1
};

static std::vector<Vertex> ex_44_cube_vertices = {
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

static std::vector<GLuint> ex_44_cube_indices = {
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

static Shader *shader_posprocessing;
static Shader *shader_g_buffer;
static Shader *shader_debug;
static Shader *shader_object;

static FrameBuffer<Texture2D> * g_buffer;

// render buffer
static FrameBuffer<Texture2D> *render_buffer;
// buffer de tela
static FrameBuffer<Texture2D> *screen_buffer;
// buffer shadow mapping
static std::vector< FrameBuffer<TextureCube> * > shadow_buffer;

static UBO *ubo_matrices;
static UBO *ubo_camera;


static Mesh *mesh_screen;
static Mesh *mesh_screen_debug;

Model *model_obj;

static std::vector<Shader*> config_shader;

static std::vector<ModelTransform> cube_models = {
    ModelTransform(glm::vec3( 0.0f, 2.5f, 0.0), glm::vec3(0.5f)),
    ModelTransform(glm::vec3( 0.0f, -4.0f, 0.8), glm::vec3(2.0f, 0.5, 2.0)),
    ModelTransform(glm::vec3( 2.0f, 0.0f, 2.5), glm::vec3(0.6f)),
    ModelTransform(glm::vec3( 0.0f, 0.0f, -2.5), glm::vec3(0.6f)),
    ModelTransform(glm::vec3( 0.0f, 0.0f, 2.5), glm::vec3(0.3f)),
    ModelTransform(glm::vec3( 0.0f, -2.5f, 0.0), glm::vec3(0.3f)),
    ModelTransform(glm::vec3( -2.5f, 0.0f, 0.0), glm::vec3(0.3f)),
    ModelTransform(glm::vec3( -2.5f, 0.0f, 2.5), glm::vec3(0.5), glm::vec3(1.0, 0.0, 1.0), 60.0f),
    ModelTransform(glm::vec3( 2.5f, 2.5f, 2.0), glm::vec3(0.5), glm::vec3(1.0, 0.0, 1.0), 60.0f),
    ModelTransform(glm::vec3( 3.5f, 3.5f, 2.5), glm::vec3(0.9), glm::vec3(1.0, 0.0, 1.0), 60.0f)
};


static void clearGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

static void loadScene(GLFWwindow* window, const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    camera->setCamSpeed(25.0f);

    shader_posprocessing = new Shader("glsl/ex_45/posprocessing.vs", "glsl/ex_45/posprocessing.fs");

    shader_g_buffer = new Shader("glsl/ex_45/g_buffer.vs", "glsl/ex_45/g_buffer.fs");

    shader_debug = new Shader("glsl/ex_45/debug_depth.vs", "glsl/ex_45/debug_depth.fs");

    shader_object = new Shader("glsl/ex_12/model_loading.vs", "glsl/ex_12/model_loading.fs");

    config_shader.push_back(shader_object);

    texture_wood = new Texture2D("./resources/textures/wood.png", TextureType::DIFFUSE, true, GL_SRGB_ALPHA);

    model_obj = new Model("./resources/obj/backpack/backpack.obj");

    // msaa com multisample 4
    render_buffer = new FrameBuffer<Texture2D>(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, TextureType::FRAMEBUFFER, 2);
    // buffer de tela
    screen_buffer = new FrameBuffer<Texture2D>(width, height, GL_RGBA, GL_RGBA16F);

    std::vector<AttachmentFrameBuffer> attachment_frame_buffer;

    // buffer position
    attachment_frame_buffer.push_back(AttachmentFrameBuffer(width, height, TextureType::FRAMEBUFFER, GL_RGBA16F, GL_RGBA, GL_FLOAT));
    // buffer normal
    attachment_frame_buffer.push_back(AttachmentFrameBuffer(width, height, TextureType::FRAMEBUFFER, GL_RGBA16F, GL_RGBA, GL_FLOAT));
    // buffer color + specular
    attachment_frame_buffer.push_back(AttachmentFrameBuffer(width, height, TextureType::FRAMEBUFFER, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE));

    // g-buffer
    g_buffer = new FrameBuffer<Texture2D>(attachment_frame_buffer);

    std::vector<Texture2D*> plane_textures = {
        texture_wood
    };

    std::vector<Texture2D*> cube_textures {
        texture_cube
    };

    std::vector<TextureCube*> shadow_textures;

    mesh_screen = new Mesh(ex_44_quad_vertices_screen, ex_44_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);
    mesh_screen_debug = new Mesh(ex_44_quad_vertices_screen_debug, ex_44_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);

    ubo_matrices = new UBO("Matrices", 2 * sizeof(glm::mat4), 0);
    ubo_camera = new UBO("Camera", sizeof(glm::vec3), 2);

    /// global matrices
    {
        glm::mat4 _projection = camera->getPerspectiveMatrix(width, height, 0.1f, 100.0f);
        glm::mat4 _view = camera->getViewMatrix();

        ubo_matrices->UBOSubBuffer(glm::value_ptr(_projection), 0, sizeof(glm::mat4));
        ubo_matrices->UBOSubBuffer(glm::value_ptr(_view), sizeof(glm::mat4), sizeof(glm::mat4));
    }

    /// global camera
    {
        ubo_camera->UBOSubBuffer(glm::value_ptr(camera->getCamPos()), 0, sizeof(glm::vec3));
    }

    {
        shader_object->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_object->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());
    }
}

static void updateScene(GLFWwindow* window, const int width, const int height)
{
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

    {
        shader_posprocessing->use();
        shader_posprocessing->setInt("screen_texture", 0);
    }

    /// Global lights
    {
        for(auto *shader : config_shader) {
            shader->use();
        }
    }
}

static void renderScene(GLFWwindow* window, const int width, const int height, Shader *_shader)
{
    Shader *used = _shader ? _shader : shader_object;

    used->use();

    glm::mat4 projection = camera->getPerspectiveMatrix(width, height);
    glm::mat4 view = camera->getViewMatrix();

    used->setMatrix4fv("projection", glm::value_ptr(projection));
    used->setMatrix4fv("view", glm::value_ptr(view));

    for(short x = -2; x < 2; ++x) {
        for(short z = -2; z < 2; ++z) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3((float)x * -5.0f, 0.0f, (float)z * -5.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            used->setMatrix4fv("model", glm::value_ptr(model));
            model_obj->draw(used);
        }
    }
}

static void renderProcessing(GLFWwindow* window, const int width, const int height)
{
    // copia o buffer da msaa para o buffer da tela
    render_buffer->bind(GL_READ_FRAMEBUFFER);
    screen_buffer->bind(GL_DRAW_FRAMEBUFFER);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, render_buffer->getWidth(), render_buffer->getHeight(), 0, 0, screen_buffer->getWidth(), screen_buffer->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    render_buffer->unbind();

    // pos-processing final
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        shader_posprocessing->use();
        screen_buffer->getTexture(0)->bind(GL_TEXTURE0);
        mesh_screen->draw(shader_posprocessing);
    }

    render_buffer->unbind();
}

static void renderProcessingDebug(GLFWwindow* window, const int width, const int height)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader_debug->use();
    render_buffer->getTexture(0)->bind(GL_TEXTURE0);
    mesh_screen_debug->draw(shader_debug);
}

static void processInput(GLFWwindow *window, float delta_time)
{
    static float waiting_time = 0.0f;

    camera->processInput(window, delta_time);

    waiting_time += delta_time;
}

static void mouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    camera->mouseCallback(window, x_pos, y_pos);
}

static void scrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    camera->scrollCallback(window, x_offset, y_offset);
}

static void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int run_045(const int width, const int height)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadScene(window, width, height);

    while(!glfwWindowShouldClose(window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        updateScene(window, width, height);
        updateShader(window, width, height);

        // g-buffer render
        /*{
            g_buffer->bind();
            glViewport(0, 0, g_buffer->getWidth(), g_buffer->getHeight());
            clearGL();
            shader_g_buffer->use();
            renderScene(window, width, height, shader_g_buffer);
            g_buffer->unbind();
        }*/

        {
            render_buffer->bind();
            glViewport(0, 0, screen_buffer->getWidth(), screen_buffer->getHeight());
            clearGL();
            renderScene(window, width, height);
            render_buffer->unbind();
        }

        renderProcessing(window, width, height);
        renderProcessingDebug(window, width, height);


        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader_posprocessing;
    delete shader_object;

    delete mesh_screen;
    delete camera;

    delete texture_wood;

    glfwTerminate();
    return 0;
}
