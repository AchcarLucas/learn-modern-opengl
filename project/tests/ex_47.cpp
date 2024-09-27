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

static void clearGL(GLbitfield bit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 1.0f);

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
static Camera *camera = new Camera(glm::vec3(25.0f, 25.0f, -25.0f), glm::vec3(0.0f, 0.0f, 0.0f));

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

static std::vector<Vertex> ex_44_quad_vertices_screen_debug_1 = {
    Vertex(glm::vec3(-0.25f,  0.0f, 0.0f) + glm::vec3(0.75f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.25f, -0.25f, 0.0f) + glm::vec3(0.75f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.0f, -0.25f, 0.0f) + glm::vec3(0.75f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.0f,  0.0f, 0.0f) + glm::vec3(0.75f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_44_quad_vertices_screen_debug_2 = {
    Vertex(glm::vec3(-0.25f,  0.0f, 0.0f) + glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.25f, -0.25f, 0.0f) + glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.0f, -0.25f, 0.0f) + glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.0f,  0.0f, 0.0f) + glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_44_quad_vertices_screen_debug_3 = {
    Vertex(glm::vec3(-0.25f,  0.0f, 0.0f) + glm::vec3(1.0f,  0.75f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.25f, -0.25f, 0.0f) + glm::vec3(1.0f,  0.75f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.0f, -0.25f, 0.0f) + glm::vec3(1.0f,  0.75f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.0f,  0.0f, 0.0f) + glm::vec3(1.0f,  0.75f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_44_quad_vertices_screen_debug_4 = {
    Vertex(glm::vec3(-0.25f,  0.0f, 0.0f) + glm::vec3(0.75f,  0.75f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.25f, -0.25f, 0.0f) + glm::vec3(0.75f,  0.75f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.0f, -0.25f, 0.0f) + glm::vec3(0.75f,  0.75f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.0f,  0.0f, 0.0f) + glm::vec3(0.75f,  0.75f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
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

static Shader *shader_blur;
static Shader *shader_bloom;
static Shader *shader_g_buffer;
static Shader *shader_debug;
static Shader *shader_object;
static Shader *shader_light;
static Shader *shader_deferred;

// blur buffer
static FrameBuffer<Texture2D> *blur_buffer;
// bloom buffer
static FrameBuffer<Texture2D> *bloom_buffer;
// g-buffer
static FrameBuffer<Texture2D> *g_buffer;
// render buffer
static FrameBuffer<Texture2D> *render_buffer;
// buffer shadow mapping
static std::vector< FrameBuffer<TextureCube> * > shadow_buffer;

static UBO *ubo_matrices;
static UBO *ubo_camera;

static Mesh *mesh_screen;
static Mesh *mesh_light;
static Mesh *mesh_screen_debug[4];

static Model *model_obj;

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

static bool light_enabled = true;

static int steps = 10;

static std::vector<glm::vec3> light_positions;
static std::vector<glm::vec4> light_colors;

static void clearGL(GLbitfield bit, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    glClearColor(r, g, b, a);
    glClear(bit);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

static void loadScene(GLFWwindow* window, const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    light_positions.push_back(glm::vec3(0, 10, 0));
    light_colors.push_back(glm::vec4(1.0, 1.0, 1.0, 50.0f));

    camera->setCamSpeed(25.0f);

    shader_blur = new Shader("glsl/ex_47/blur.vs", "glsl/ex_47/blur.fs");
    shader_bloom = new Shader("glsl/ex_47/bloom.vs", "glsl/ex_47/bloom.fs");

    shader_g_buffer = new Shader("glsl/ex_47/g_buffer.vs", "glsl/ex_47/g_buffer.fs");
    shader_debug = new Shader("glsl/ex_47/debug_depth.vs", "glsl/ex_47/debug_depth.fs");
    shader_object = new Shader("glsl/ex_47/model_loading.vs", "glsl/ex_47/model_loading.fs");
    shader_deferred = new Shader("glsl/ex_47/deferred_shading.vs", "glsl/ex_47/deferred_shading.fs");
    shader_light = new Shader("glsl/ex_47/light.vs", "glsl/ex_47/light.fs");

    config_shader.push_back(shader_object);
    config_shader.push_back(shader_deferred);

    model_obj = new Model("./resources/obj/backpack/backpack.obj");
    // model_obj = new Model("./resources/obj/planet/planet.obj");

    // blur buffer
    blur_buffer = new FrameBuffer<Texture2D>(width, height);
    // bloom buffer
    bloom_buffer = new FrameBuffer<Texture2D>(width, height);
    // render buffer
    render_buffer = new FrameBuffer<Texture2D>(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, TextureType::FRAMEBUFFER, 2);

    std::vector<AttachmentFrameBuffer> attachment_frame_buffer;

    // buffer position
    attachment_frame_buffer.push_back(AttachmentFrameBuffer(GL_RGBA16F, GL_RGBA, GL_FLOAT));
    // buffer normal
    attachment_frame_buffer.push_back(AttachmentFrameBuffer(GL_RGBA16F, GL_RGBA, GL_FLOAT));
    // buffer color + specular
    attachment_frame_buffer.push_back(AttachmentFrameBuffer(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE));

    // g-buffer
    g_buffer = new FrameBuffer<Texture2D>(width, height, attachment_frame_buffer);

    std::vector<TextureCube*> shadow_textures;

    mesh_screen = new Mesh(ex_44_quad_vertices_screen, ex_44_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);
    mesh_light = new Mesh(ex_44_cube_vertices, ex_44_cube_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);

    mesh_screen_debug[0] = new Mesh(ex_44_quad_vertices_screen_debug_1, ex_44_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);
    mesh_screen_debug[1] = new Mesh(ex_44_quad_vertices_screen_debug_2, ex_44_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);
    mesh_screen_debug[2] = new Mesh(ex_44_quad_vertices_screen_debug_3, ex_44_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);
    mesh_screen_debug[3] = new Mesh(ex_44_quad_vertices_screen_debug_4, ex_44_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);

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

        shader_g_buffer->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_g_buffer->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());

        shader_light->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
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
    /// Global lights
    {
        for(auto *shader : config_shader) {
            shader->use();

            for(unsigned i = 0; i < light_positions.size(); ++i) {
                string s_lights = "lights[" + std::to_string(i) + "].";
                shader->setUniform3fv(s_lights + "position", glm::value_ptr(light_positions[i]));
                shader->setUniform3fv(s_lights + "color", glm::value_ptr(light_colors[i]));
                shader->setFloat(s_lights + "linear",  100.0f);
                shader->setFloat(s_lights + "quadratic", 2.1f);
                shader->setBool(s_lights + "enabled", light_enabled);
            }
        }
    }
}

static void renderScene(GLFWwindow* window, const int width, const int height, Shader *_shader)
{
    Shader *used = _shader ? _shader : shader_object;

    used->use();

    for(short x = -2; x <= 2; ++x) {
        for(short z = -2; z <= 2; ++z) {
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
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // pos-processing blur
    {
        blur_buffer->bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        bool _horizontal = true;

        for(int s = 0; s < steps * 2; ++s) {
            shader_blur->use();
            shader_blur->setInt("_texture", 0);
            shader_blur->setBool("horizontal", _horizontal);

            if(s == 0)
                render_buffer->getTexture(1)->bind(GL_TEXTURE0);
            else
                blur_buffer->getTexture(0)->bind(GL_TEXTURE0);

            mesh_screen->draw(shader_blur);

            _horizontal = !_horizontal;
        }

        blur_buffer->unbind();
    }

    // pos-processing bloom
    {
        bloom_buffer->bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        shader_bloom->use();

        shader_bloom->setInt("render_buffer", 0);
        shader_bloom->setInt("blur_buffer", 1);
        shader_bloom->setBool("bloom_enabled", true);

        render_buffer->getTexture(0)->bind(GL_TEXTURE0);
        blur_buffer->getTexture(0)->bind(GL_TEXTURE1);

        mesh_screen->draw(shader_bloom);
        bloom_buffer->unbind();
    }

    // pos-processing deferred
    {
        clearGL();
        shader_deferred->use();
        shader_deferred->setInt("gPosition", 0);
        shader_deferred->setInt("gNormal", 1);
        shader_deferred->setInt("gAlbedoSpec", 2);
        shader_deferred->setInt("gBloom", 3);
        g_buffer->getTexture(0)->bind(GL_TEXTURE0);
        g_buffer->getTexture(1)->bind(GL_TEXTURE1);
        g_buffer->getTexture(2)->bind(GL_TEXTURE2);
        bloom_buffer->getTexture(0)->bind(GL_TEXTURE3);
        mesh_screen->draw(shader_deferred);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

static void renderProcessingDebug(GLFWwindow* window, const int width, const int height)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader_debug->use();
    shader_debug->setInt("_texture", 0);
    shader_debug->setBool("isSpecular", false);
    g_buffer->getTexture(0)->bind(GL_TEXTURE0);
    mesh_screen_debug[0]->draw(shader_debug);

    shader_debug->setInt("_texture", 0);
    shader_debug->setBool("isSpecular", false);
    g_buffer->getTexture(1)->bind(GL_TEXTURE0);
    mesh_screen_debug[1]->draw(shader_debug);

    shader_debug->setInt("_texture", 0);
    shader_debug->setBool("isSpecular", false);
    g_buffer->getTexture(2)->bind(GL_TEXTURE0);
    mesh_screen_debug[2]->draw(shader_debug);

    shader_debug->setInt("_texture", 0);
    shader_debug->setBool("isSpecular", true);
    g_buffer->getTexture(2)->bind(GL_TEXTURE0);
    mesh_screen_debug[3]->draw(shader_debug);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

static void renderLight(GLFWwindow* window, const int width, const int height)
{
    if(light_enabled)
    {
        for(unsigned i = 0; i < light_positions.size(); ++i)
        {
            shader_light->use();

            glm::mat4 model(1.0f);
            model = glm::translate(model, light_positions[i]);
            model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));

            shader_light->setUniform4fv("color", glm::value_ptr(light_colors[i]));
            shader_light->setMatrix4fv("model", glm::value_ptr(model));

            mesh_light->draw(shader_light);
        }
    }
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

int run_047(const int width, const int height)
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
        {
            glDisable(GL_BLEND);
            g_buffer->bind();
            glViewport(0, 0, g_buffer->getWidth(), g_buffer->getHeight());
            clearGL();
            renderScene(window, width, height, shader_g_buffer);
            g_buffer->unbind();
            glEnable(GL_BLEND);
        }

        renderProcessing(window, width, height);

        /*
         * Copia o DEPTH_BUFFER do g_buffer para o depth buffer principal
         */
        {
            glDisable(GL_BLEND);
            g_buffer->bind(GL_READ_FRAMEBUFFER);
            render_buffer->bind(GL_DRAW_FRAMEBUFFER);
            glBlitFramebuffer(0, 0, width, height, 0, 0, g_buffer->getWidth(), g_buffer->getHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glEnable(GL_BLEND);
        }

        // buffer render
        {
            glDisable(GL_BLEND);
            render_buffer->bind();
            glViewport(0, 0, render_buffer->getWidth(), render_buffer->getHeight());
            clearGL(GL_COLOR_BUFFER_BIT);
            renderLight(window, width, height);
            render_buffer->unbind();
            glEnable(GL_BLEND);
        }

        renderProcessingDebug(window, width, height);

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader_blur;
    delete shader_bloom;
    delete shader_g_buffer;
    delete shader_debug;
    delete shader_object;
    delete shader_light;
    delete shader_deferred;

    delete blur_buffer;
    delete bloom_buffer;
    delete render_buffer;
    delete g_buffer;

    delete ubo_matrices;
    delete ubo_camera;

    delete mesh_screen;
    delete mesh_light;

    delete mesh_screen_debug[0];
    delete mesh_screen_debug[1];
    delete mesh_screen_debug[2];
    delete mesh_screen_debug[3];

    delete model_obj;

    glfwTerminate();
    return 0;
}
