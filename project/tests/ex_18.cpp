#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "framebuffer.hpp"

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static void processInput(GLFWwindow *, float);
static void mouseCallback(GLFWwindow*, double, double);
static void scrollCallback(GLFWwindow*, double, double);

// camera class
static Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

static std::vector<Vertex> ex_18_quad_vertices = {
    Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_18_floor_vertices = {
    Vertex(glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(7.0f, 7.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(7.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 7.0f))
};

static std::vector<GLuint> ex_18_indices = {
    0, 1, 3,
    1, 2, 3
};

static std::vector<GLuint> ex_18_quad_indices = {
    3, 1, 0,
    3, 2, 1
};

static std::vector<Vertex> ex_18_cube_vertices = {
    // Back face
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f)),
    // Front face
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
    // Left face
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
    // Right face
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
    // Bottom face
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),
    // Top face
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, 0.5f),  glm::vec2(0.0f, 0.0f)),
};

static std::vector<Vertex> ex_18_cube_vertices_skybox = {
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),

    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),

    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),

    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),

    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),

    Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f,  1.0f))
};

static std::vector<GLuint> ex_18_cube_indices = {
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

int run_018(const int width, const int height)
{
    camera->setCamPos(glm::vec3(0.0f, 2.0f, 8.0f));

    Shader *shader = new Shader("glsl/ex_18/model_loading.vs", "glsl/ex_18/model_loading.fs");
    Shader *shader_cube = new Shader("glsl/ex_18/cube.vs", "glsl/ex_18/cube.fs");
    Shader *shader_skybox = new Shader("glsl/ex_18/skybox.vs", "glsl/ex_18/skybox.fs");
    Shader *posprocessing_shader = new Shader("glsl/ex_18/posprocessing.vs", "glsl/ex_18/posprocessing.fs");

    std::vector<Texture2D*> textures;
    textures.push_back(new Texture2D("resources/textures/floor.png", TextureType::ALBEDO));
    textures.push_back(new Texture2D("resources/textures/container.jpg", TextureType::ALBEDO));

    std::vector<Texture2D*> floor_texture;
    floor_texture.push_back(textures[0]);
    Mesh *mesh_floor = new Mesh(ex_18_floor_vertices, ex_18_indices, floor_texture, VERTEX_TYPE::ATTRIB_PNT);

    std::vector<Texture2D*> cube_texture;
    cube_texture.push_back(textures[1]);
    Mesh *mesh_cube = new Mesh(ex_18_cube_vertices, ex_18_cube_indices, cube_texture, VERTEX_TYPE::ATTRIB_PT);

    /////////////////////////

    std::string dir = "resources/textures/skybox_1/";

    vector<std::string> faces = {
        std::string(dir + "right.jpg"),
        std::string(dir + "left.jpg"),
        std::string(dir + "top.jpg"),
        std::string(dir + "bottom.jpg"),
        std::string(dir + "front.jpg"),
        std::string(dir + "back.jpg")
    };

    TextureCube *texture_cube_skybox = new TextureCube(faces, false);
    Mesh *mesh_cube_skybox = new Mesh(ex_18_cube_vertices_skybox, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_P);

    shader_skybox->use();
    shader_skybox->setInt("skybox", 0);

    // quad posprocessing
    Mesh *mesh_posprocessing = new Mesh(ex_18_quad_vertices, ex_18_quad_indices, std::vector<Texture2D*>());

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<glm::vec3> cube_positions = {
        glm::vec3(2.0f, 0.0f, -3.0f),
        glm::vec3(-2.0f, 0.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };

    // Framebuffer
    FrameBuffer<Texture2D> *framebuffer = new FrameBuffer<Texture2D>(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);

    while(!glfwWindowShouldClose(window)) {
        framebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glm::mat4 projection = camera->getPerspectiveMatrix(width, height);
        glm::mat4 view = camera->getViewMatrix();

        // skybox
        {
            glDepthFunc(GL_LEQUAL);
            glm::mat4 view_modify = glm::mat4(glm::mat3(camera->getViewMatrix()));
            shader_skybox->use();
            shader_skybox->setMatrix4fv("projection", glm::value_ptr(projection));
            shader_skybox->setMatrix4fv("view", glm::value_ptr(view_modify));

            texture_cube_skybox->bind(GL_TEXTURE0);
            mesh_cube_skybox->draw(shader_skybox);

            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }

        // floor
        {
            shader->use();

            shader->setMatrix4fv("projection", glm::value_ptr(projection));
            shader->setMatrix4fv("view", glm::value_ptr(view));

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too big for our scene, so scale it down
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            shader->setMatrix4fv("model", glm::value_ptr(model));

            mesh_floor->draw(shader);
        }

        // cubes
        {
            shader_cube->use();
            shader_cube->setMatrix4fv("projection", glm::value_ptr(projection));
            shader_cube->setMatrix4fv("view", glm::value_ptr(view));

            for(auto &cube : cube_positions) {


                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(cube)); // translate it down so it's at the center of the scene
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

                shader_cube->setMatrix4fv("model", glm::value_ptr(model));

                mesh_cube->draw(shader_cube);
            }
        }

        // pos-processing
        {
            framebuffer->unbind();
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glDisable(GL_DEPTH_TEST);

            posprocessing_shader->use();
            posprocessing_shader->setInt("screenTexture", 0);
            framebuffer->getTexture()->bind(GL_TEXTURE0);
            mesh_posprocessing->draw(posprocessing_shader);
        }

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader;
    delete shader_cube;
    delete shader_skybox;
    delete posprocessing_shader;

    delete mesh_floor;
    delete mesh_cube;

    delete camera;

    delete framebuffer;

    for (auto& texture : textures) {
        delete texture;
    }

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
