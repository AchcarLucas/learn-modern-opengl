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
static Camera *camera = new Camera(glm::vec3(5.0f, 5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

static float _gamma = 2.2f;

static std::vector<Vertex> ex_42_quad_vertices_screen = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_42_quad_vertices_plane = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(10.0f, 10.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(10.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 10.0f))
};

static std::vector<GLuint> ex_42_quad_indices = {
    3, 1, 0,
    3, 2, 1
};

static std::vector<Vertex> ex_42_cube_vertices = {
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

static std::vector<GLuint> ex_42_cube_indices = {
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
static Shader *shader_plane;
static Shader *shader_cube;
static Shader *shader_light;
static Shader *shader_shadow;

// msaa com multisample 4
static FrameBuffer<Texture2D> *msaa_buffer;
// buffer de tela
static FrameBuffer<Texture2D> *screen_buffer;
// buffer shadow mapping
static std::vector< FrameBuffer<TextureCube> * > shadow_buffer;

static UBO *ubo_matrices;
static UBO *ubo_camera;

static Mesh *mesh_screen;
static Mesh *mesh_plane;
static Mesh *mesh_cube;
static Mesh *mesh_light;

static std::vector<PointLight *> p_light;

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

static std::vector<glm::vec4> light_position = {
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
    glm::vec4(-0.5f, 0.0f, -0.5f, 1.0f),
    glm::vec4(0.5f, 1.0f, 0.5f, 1.0f)
};

#define MAX_DEPTH_CUBEMAP 7

static bool light_enabled = true;

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

    float near_light = 1.0f;
    float far_light = 100.0f;

    for(std::vector<glm::vec4>::iterator it = light_position.begin(); it != light_position.end(); ++it) {
        p_light.push_back(new PointLight(*it, width, height, near_light, far_light));
    }

    shader_screen = new Shader("glsl/ex_42/posprocessing.vs", "glsl/ex_42/posprocessing.fs");
    shader_plane = new Shader("glsl/ex_42/object.vs", "glsl/ex_42/object.fs");
    shader_cube = new Shader("glsl/ex_42/object.vs", "glsl/ex_42/object.fs");
    shader_light = new Shader("glsl/ex_42/light.vs", "glsl/ex_42/light.fs");
    shader_shadow = new Shader("glsl/ex_42/depth.vs", "glsl/ex_42/depth.fs", "glsl/ex_42/depth.gs");

    config_shader.push_back(shader_cube);
    config_shader.push_back(shader_plane);

    texture_wood = new Texture2D("./resources/textures/wood.png", TextureType::DIFFUSE, true, GL_SRGB);
    texture_cube = new Texture2D("./resources/textures/container2.png", TextureType::DIFFUSE, true, GL_SRGB);

    // msaa com multisample 4
    msaa_buffer = new FrameBuffer<Texture2D>(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, 4);
    // buffer de tela
    screen_buffer = new FrameBuffer<Texture2D>(width, height);

    // buffers shadow mapping
    for(unsigned i = 0; i < MAX_DEPTH_CUBEMAP; ++i) {
        shadow_buffer.push_back(new FrameBuffer<TextureCube>(1024, 1024, GL_NONE, GL_NONE, TextureType::FRAMEBUFFER_DEPTH_CUBEMAP));
    }

    std::vector<Texture2D*> plane_textures = {
        texture_wood
    };

    std::vector<Texture2D*> cube_textures {
        texture_cube
    };

    std::vector<TextureCube*> shadow_textures;

    for(std::vector< FrameBuffer<TextureCube> * >::iterator it = shadow_buffer.begin(); it != shadow_buffer.end(); ++it) {
        shadow_textures.push_back((*it)->getTexture());
    }

    mesh_screen = new Mesh(ex_42_quad_vertices_screen, ex_42_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);

    mesh_plane = new Mesh(ex_42_quad_vertices_plane, ex_42_quad_indices, plane_textures, shadow_textures, VERTEX_TYPE::ATTRIB_PNT);
    mesh_cube = new Mesh(ex_42_cube_vertices, ex_42_cube_indices, cube_textures, shadow_textures, VERTEX_TYPE::ATTRIB_PNT);
    mesh_light = new Mesh(ex_42_cube_vertices, ex_42_cube_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);

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

    {
        shader_plane->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_plane->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());

        shader_cube->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_cube->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());

        shader_light->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
    }
}

static void updateScene(GLFWwindow* window, const int width, const int height)
{
    for(unsigned i = 0; i < p_light.size(); ++i)
    {
        light_position[i] = light_position[i] * glm::rotate(glm::mat4(1.0f), glm::radians(delta_time) * 100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        p_light[i]->setPosition(light_position[i]);
    }

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
        shader_screen->setFloat("gamma", _gamma);
        shader_screen->setInt("screen_texture", 0);

        shader_cube->use();
        shader_cube->setFloat("gamma", _gamma);

        shader_plane->use();
        shader_plane->setFloat("gamma", _gamma);

        first = false;
    }

    /// Global lights
    {
        for(auto *shader : config_shader) {
            shader->use();

            // Light 1
            shader->setUniform4fv("lights[0].position", glm::value_ptr(light_position[0]));
            shader->setUniform4fv("lights[0].ambient", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));
            shader->setUniform4fv("lights[0].diffuse", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));
            shader->setUniform4fv("lights[0].specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));

            shader->setUniform3fv("lights[0].spot_direction", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

            shader->setFloat("lights[0].constant_attenuation", 0.0f);
            shader->setFloat("lights[0].linear_attenuation", 0.0f);
            shader->setFloat("lights[0].quadratic_attenuation", 0.0f);

            shader->setFloat("lights[0].spot_cutoff", 0.0f);
            shader->setFloat("lights[0].spot_exponent", 0.0f);

            shader->setFloat("lights[0].near_plane", p_light[0]->getNearPlane());
            shader->setFloat("lights[0].far_plane", p_light[0]->getFarPlane());

            shader->setInt("lights[0].cubemap_index", 1);

            shader->setBool("lights[0].enabled", light_enabled);

            // Light 2
            shader->setUniform4fv("lights[1].position", glm::value_ptr(light_position[1]));
            shader->setUniform4fv("lights[1].ambient", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));
            shader->setUniform4fv("lights[1].diffuse", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));
            shader->setUniform4fv("lights[1].specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));

            shader->setUniform3fv("lights[1].spot_direction", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

            shader->setFloat("lights[1].constant_attenuation", 0.0f);
            shader->setFloat("lights[1].linear_attenuation", 0.0f);
            shader->setFloat("lights[1].quadratic_attenuation", 0.0f);

            shader->setFloat("lights[1].spot_cutoff", 0.0f);
            shader->setFloat("lights[1].spot_exponent", 0.0f);

            shader->setFloat("lights[1].near_plane", p_light[1]->getNearPlane());
            shader->setFloat("lights[1].far_plane", p_light[1]->getFarPlane());

            shader->setInt("lights[1].cubemap_index", 2);

            shader->setBool("lights[1].enabled", light_enabled);

            // Light 3
            shader->setUniform4fv("lights[2].position", glm::value_ptr(light_position[2]));
            shader->setUniform4fv("lights[2].ambient", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));
            shader->setUniform4fv("lights[2].diffuse", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));
            shader->setUniform4fv("lights[2].specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f) * 20.0f));

            shader->setUniform3fv("lights[2].spot_direction", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

            shader->setFloat("lights[2].constant_attenuation", 0.0f);
            shader->setFloat("lights[2].linear_attenuation", 0.0f);
            shader->setFloat("lights[2].quadratic_attenuation", 0.0f);

            shader->setFloat("lights[2].spot_cutoff", 0.0f);
            shader->setFloat("lights[2].spot_exponent", 0.0f);

            shader->setFloat("lights[2].near_plane", p_light[2]->getNearPlane());
            shader->setFloat("lights[2].far_plane", p_light[2]->getFarPlane());

            shader->setInt("lights[2].cubemap_index", 3);

            shader->setBool("lights[2].enabled", light_enabled);
        }
    }
}

static void renderScene(GLFWwindow* window, const int width, const int height, Shader *_shader)
{
    float size_room = 8.0f;

    Shader *used = _shader ? _shader : shader_plane;

    /// Draw floor
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -size_room, 0.0f));
        model = glm::scale(model, glm::vec3(size_room, size_room, size_room));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);
    }

    /// Draw roof
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, size_room, 0.0f));
        model = glm::scale(model, glm::vec3(size_room, size_room, size_room));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);
    }

    /// Draw wall
    {
        // left

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, size_room));
        model = glm::scale(model, glm::vec3(size_room, size_room, size_room));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);

        // right

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -size_room));
        model = glm::scale(model, glm::vec3(size_room, size_room, size_room));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);

        // front

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(size_room, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(size_room, size_room, size_room));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);

        // back

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-size_room, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(size_room, size_room, size_room));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);
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
    if(light_enabled)
    {
        for(std::vector<glm::vec4>::iterator it = light_position.begin(); it != light_position.end(); ++it)
        {
            shader_light->use();

            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(*it));
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

            shader_light->setMatrix4fv("model", glm::value_ptr(model));

            mesh_light->draw(shader_light);
        }
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
        glDisable(GL_CULL_FACE);

        shader_screen->use();
        screen_buffer->getTexture()->bind(GL_TEXTURE0);
        mesh_screen->draw(shader_screen);
    }
}

static void renderProcessingDebug(GLFWwindow* window, const int width, const int height)
{
    return;
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

static void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int run_042(const int width, const int height)
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

        /// shader shadow casting
        for(unsigned i = 0; i < p_light.size(); ++i) {
            shadow_buffer[i]->bind();
            glViewport(0, 0, shadow_buffer[i]->getWidth(), shadow_buffer[i]->getHeight());
            clearGL();
            p_light[i]->settingShader(shader_shadow);
            renderScene(window, width, height, shader_shadow);
            shadow_buffer[i]->unbind();
        }

        {
            msaa_buffer->bind();
            glViewport(0, 0, screen_buffer->getWidth(), screen_buffer->getHeight());
            clearGL();
            renderScene(window, width, height);
            renderLight(window, width, height);
        }

        renderProcessing(window, width, height);
        renderProcessingDebug(window, width, height);

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete texture_wood;
    delete texture_cube;

    delete shader_screen;
    delete shader_plane;
    delete shader_cube;
    delete shader_light;
    delete shader_shadow;

    delete msaa_buffer;
    delete screen_buffer;

    for(std::vector< FrameBuffer<TextureCube> * >::iterator it = shadow_buffer.begin(); it != shadow_buffer.end(); ++it)
        delete (*it);

    shadow_buffer.clear();

    delete ubo_matrices;
    delete ubo_camera;

    delete mesh_screen;
    delete mesh_plane;
    delete mesh_light;

    for(std::vector<PointLight *>::iterator it = p_light.begin(); it != p_light.end(); ++it)
        delete (*it);

    p_light.clear();

    glfwTerminate();
    return 0;
}
