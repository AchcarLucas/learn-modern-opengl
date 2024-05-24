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
static void renderLight(GLFWwindow*, const int, const int);
static void renderProcessing(GLFWwindow*, const int, const int);
static void renderProcessingDebug(GLFWwindow*, const int, const int);

static void processInput(GLFWwindow *, float);
static void mouseCallback(GLFWwindow*, double, double);
static void scrollCallback(GLFWwindow*, double, double);

// camera class
static Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

static float _gamma = 2.2f;

static std::vector<Vertex> ex_43_quad_vertices_screen = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_43_quad_vertices_plane = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<GLuint> ex_43_quad_indices = {
    3, 1, 0,
    3, 2, 1
};

static std::vector<Vertex> ex_43_cube_vertices = {
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

static std::vector<GLuint> ex_43_cube_indices = {
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
static Shader *shader_text;

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
static Mesh *mesh_light;

static RenderText *render_text_screen;

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
    glm::vec4(0.0f,  0.0f, 49.5f, 1.0f),
    glm::vec4(-1.4f, -1.9f, 9.0f, 1.0f),
    glm::vec4(0.0f, -1.8f, 4.0f, 1.0f),
    glm::vec4(0.8f, -1.7f, 6.0f, 1.0f)
};

static std::vector<glm::vec3> light_color = {
    glm::vec3(200.0f, 200.0f, 200.0f),
    glm::vec3(0.1f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.2f),
    glm::vec3(0.0f, 0.1f, 0.0f),
};

static bool light_enabled = true;
static bool hdr_enabled = true;
static float exposure = 1.0f;

#define MAX_DEPTH_CUBEMAP 7

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

    shader_screen = new Shader("glsl/ex_43/posprocessing.vs", "glsl/ex_43/posprocessing.fs");
    shader_plane = new Shader("glsl/ex_43/object.vs", "glsl/ex_43/object.fs");
    shader_cube = new Shader("glsl/ex_43/object.vs", "glsl/ex_43/object.fs");
    shader_light = new Shader("glsl/ex_43/light.vs", "glsl/ex_43/light.fs");
    shader_shadow = new Shader("glsl/ex_43/depth.vs", "glsl/ex_43/depth.fs", "glsl/ex_43/depth.gs");
    shader_text = new Shader("glsl/text_sdf/text_sdf.vs", "glsl/text_sdf/text_sdf.fs");

    config_shader.push_back(shader_cube);
    config_shader.push_back(shader_plane);

    texture_wood = new Texture2D("./resources/textures/wood.png", TextureType::DIFFUSE, true, GL_SRGB_ALPHA);

    render_text_screen = new RenderText("fonts/arial.ttf", width, height, 0, 32, TextType::DRAW_TO_SCREEN, true);

    // msaa com multisample 4
    msaa_buffer = new FrameBuffer<Texture2D>(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, 4);
    // buffer de tela
    screen_buffer = new FrameBuffer<Texture2D>(width, height, GL_RGBA, GL_RGBA16F);

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

    mesh_screen = new Mesh(ex_43_quad_vertices_screen, ex_43_quad_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);

    mesh_plane = new Mesh(ex_43_quad_vertices_plane, ex_43_quad_indices, plane_textures, shadow_textures, VERTEX_TYPE::ATTRIB_PNT);
    mesh_light = new Mesh(ex_43_cube_vertices, ex_43_cube_indices, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PNT);

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
        shader_plane->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_plane->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());

        shader_cube->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
        shader_cube->setUniformBlockBinding(ubo_camera->getName(), ubo_camera->getBinding());

        shader_light->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
    }
}

static void updateScene(GLFWwindow* window, const int width, const int height)
{
    for(unsigned i = 0; i < p_light.size(); ++i) {
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

    {
        shader_screen->use();
        shader_screen->setFloat("gamma", _gamma);
        shader_screen->setFloat("exposure", exposure);
        shader_screen->setBool("hdr_enabled", hdr_enabled);
        shader_screen->setInt("screen_texture", 0);

        shader_cube->use();
        shader_cube->setFloat("gamma", _gamma);

        shader_plane->use();
        shader_plane->setFloat("gamma", _gamma);
    }

    /// Global lights
    {
        for(auto *shader : config_shader) {
            shader->use();

            for(unsigned i = 0; i < light_position.size(); ++i) {
                string s_lights = "lights[" + std::to_string(i) + "].";
                shader->setUniform4fv(s_lights + "position", glm::value_ptr(light_position[i]));
                shader->setUniform4fv(s_lights + "ambient", glm::value_ptr(light_color[i]));
                shader->setUniform4fv(s_lights + "diffuse", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
                shader->setUniform4fv(s_lights + "specular", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));

                shader->setUniform3fv(s_lights + "spot_direction", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

                shader->setFloat(s_lights + "constant_attenuation", 0.0f);
                shader->setFloat(s_lights + "linear_attenuation", 0.0f);
                shader->setFloat(s_lights + "quadratic_attenuation", 0.0f);

                shader->setFloat(s_lights + "spot_cutoff", 0.0f);
                shader->setFloat(s_lights + "spot_exponent", 0.0f);

                shader->setFloat(s_lights + "near_plane", p_light[i]->getNearPlane());
                shader->setFloat(s_lights + "far_plane", p_light[i]->getFarPlane());

                shader->setInt(s_lights + "cubemap_index", i + 1);

                shader->setBool(s_lights + "enabled", light_enabled);
            }
        }
    }
}

static void renderScene(GLFWwindow* window, const int width, const int height, Shader *_shader)
{
    float size_room[] = {2.5f, 2.5f, 27.5f};

    Shader *used = _shader ? _shader : shader_plane;

    /// Draw floor
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::translate(model, glm::vec3(0.0f, -size_room[1], 0.0f));
        model = glm::scale(model, glm::vec3(size_room[0], size_room[1], size_room[2]));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);
    }

    /// Draw roof
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::translate(model, glm::vec3(0.0f, size_room[1], 0.0f));
        model = glm::scale(model, glm::vec3(size_room[0], size_room[1], size_room[2]));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);
    }

    /// Draw wall
    {
        // left

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, size_room[2]));
        model = glm::scale(model, glm::vec3(size_room[0], size_room[1], size_room[2]));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);

        // right

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -size_room[2]));
        model = glm::scale(model, glm::vec3(size_room[0], size_room[1], size_room[2]));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);

        // front

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::translate(model, glm::vec3(size_room[0], 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(size_room[0], size_room[1], size_room[2]));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);

        // back

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::translate(model, glm::vec3(-size_room[0], 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(size_room[0], size_room[1], size_room[2]));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        used->use();
        used->setMatrix4fv("model", glm::value_ptr(model));

        mesh_plane->draw(used);
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
    static float waiting_time = 0.0f;

    camera->processInput(window, delta_time);

    if(waiting_time > 0.2f) {
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            light_enabled = !light_enabled;
            std::cout << "light [" << (light_enabled ? "ON" : "OFF") << "]" << std::endl;
            waiting_time = 0.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
            hdr_enabled = !hdr_enabled;
            std::cout << "hdr [" << (hdr_enabled ? "ON" : "OFF") << "]" << std::endl;
            waiting_time = 0.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        {
            exposure -= 0.05f;
            exposure =  glm::clamp(exposure, 0.0f, 5.0f);
        }
        else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        {
            exposure += 0.05f;
            exposure = glm::clamp(exposure, 0.0f, 5.0f);
        }
    }

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

int run_043(const int width, const int height)
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

            // screen text render
            {
                render_text_screen->draw(shader_text,
                    string("Press L to ") +
                                (light_enabled ? string("Disabled") : string("Enabled")) +
                                string(" Light "),
                    530.0f, 530.0f, 0.5f,
                    glm::vec3(0.5, 0.8f, 0.2f));

                render_text_screen->draw(shader_text,
                    string("Press H to ") +
                                (hdr_enabled ? string("Disabled") : string("Enabled")) +
                                string(" HDR "),
                    530.0f, 500.0f, 0.5f,
                    glm::vec3(0.5, 0.8f, 0.2f));

                render_text_screen->draw(shader_text,
                string("Press '+' or '-' to ") + to_string(exposure) + string(" Exposure HDR"),
                480.0f, 470.0f, 0.5f,
                glm::vec3(0.5, 0.8f, 0.2f));
            }

            msaa_buffer->unbind();
        }

        renderProcessing(window, width, height);
        renderProcessingDebug(window, width, height);

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete render_text_screen;

    delete shader_plane;
    delete shader_screen;
    delete shader_cube;
    delete shader_light;

    delete mesh_screen;
    delete mesh_plane;

    delete camera;

    delete texture_wood;

    glfwTerminate();
    return 0;
}
