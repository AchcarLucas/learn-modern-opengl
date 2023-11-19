#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"

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

static std::vector<Vertex> ex_15_quad_vertices = {
    Vertex(glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f))
};

static std::vector<Vertex> ex_15_floor_vertices = {
    Vertex(glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(7.0f, 7.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(7.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 7.0f))
};

static std::vector<GLuint> ex_15_indices = {
    3, 1, 0,
    3, 2, 1
};

class TransparentSorted {
    public:
        TransparentSorted(glm::vec3 position, float distance) {
            this->position = position;
            this->distance = distance;
        }

        glm::vec3 getPosition() { return this->position; }
        float getDistance() { return this->distance; }

        glm::vec3 position;
        float distance;

        static bool compare(const TransparentSorted &l, const TransparentSorted &r)
        {
            return l.distance > r.distance;
        }
};

glm::mat4 billboard(glm::vec3 position, glm::vec3 camera, glm::vec3 up, glm::vec3 right) {
    glm::vec3 look = normalize(camera - position);

    glm::mat4 _transform;

    _transform[0] = glm::vec4(right, 0.0f);
    _transform[1] = glm::vec4(cross(look, right), 0.0f);
    _transform[2] = glm::vec4(look, 0.0f);
    _transform[3] = glm::vec4(position, 1.0f);

    return _transform;
}

int run_015(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    camera->setCamPos(glm::vec3(0.0f, 2.0f, 5.0f));

    Shader *grass_shader = new Shader("glsl/ex_15/grass.vs", "glsl/ex_15/grass.fs");
    Shader *floor_shader = new Shader("glsl/ex_15/floor.vs", "glsl/ex_15/floor.fs");
    Shader *window_shader = new Shader("glsl/ex_15/window.vs", "glsl/ex_15/window.fs");

    std::vector<Texture2D*> textures;

    textures.push_back(new Texture2D("resources/textures/grass.png", TextureType::ALBEDO));
    textures.push_back(new Texture2D("resources/textures/blending_transparent_window.png", TextureType::ALBEDO));
    textures.push_back(new Texture2D("resources/textures/floor.png", TextureType::ALBEDO));

    std::vector<Texture2D*> gress_texture;
    gress_texture.push_back(textures[0]);
    Mesh *mesh_gress = new Mesh(ex_15_quad_vertices, ex_15_indices, gress_texture);

    std::vector<Texture2D*> window_texture;
    window_texture.push_back(textures[1]);
    Mesh *mesh_window = new Mesh(ex_15_quad_vertices, ex_15_indices, window_texture);

    std::vector<Texture2D*> floor_texture;
    floor_texture.push_back(textures[2]);
    Mesh *mesh_floor = new Mesh(ex_15_floor_vertices, ex_15_indices, floor_texture);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vector<glm::vec3> vegetations;
    vegetations.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
    vegetations.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
    vegetations.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
    vegetations.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
    vegetations.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));


    std::vector<TransparentSorted> windows;
    windows.push_back(TransparentSorted(glm::vec3( 2.5f,  0.0f,  1.0f), 0.0f));
    windows.push_back(TransparentSorted(glm::vec3( 1.0f,  0.0f,  0.9f), 0.0f));
    windows.push_back(TransparentSorted(glm::vec3( 0.8f,  0.0f, 3.3f), 0.0f));
    windows.push_back(TransparentSorted(glm::vec3( 1.5f,  0.0f, -1.6f), 0.0f));
    windows.push_back(TransparentSorted(glm::vec3( 1.5f,  0.0f,  -4.0f), 0.0f));

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_CULL_FACE);

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glm::mat4 projection = camera->getPerspectiveMatrix(width, height);
        glm::mat4 view = camera->getViewMatrix();

        floor_shader->use();
        floor_shader->setMatrix4fv("projection", glm::value_ptr(projection));
        floor_shader->setMatrix4fv("view", glm::value_ptr(view));

        // floor
        {
            floor_shader->use();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too big for our scene, so scale it down
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            floor_shader->setMatrix4fv("model", glm::value_ptr(model));

            mesh_floor->draw(floor_shader);
        }

        grass_shader->use();
        grass_shader->setMatrix4fv("projection", glm::value_ptr(projection));
        grass_shader->setMatrix4fv("view", glm::value_ptr(view));

        window_shader->use();
        window_shader->setMatrix4fv("projection", glm::value_ptr(projection));
        window_shader->setMatrix4fv("view", glm::value_ptr(view));

        glDisable(GL_CULL_FACE);

        // grass
        {
            for(auto &vegetation : vegetations) {
                grass_shader->use();

                glm::mat4 model(1.0f);
                model = glm::translate(model, vegetation); // translate it down so it's at the center of the scene
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
                model = glm::rotate(model, glm::radians(camera->getYaw()+ 90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

                grass_shader->setMatrix4fv("model", glm::value_ptr(model));

                mesh_gress->draw(grass_shader);
            }
        }

        // window
        {
            // obtemos a distância de todos os objetos em relação a camera
            for(auto &window : windows) {
                window.distance = glm::length(camera->getCamPos() - window.position);
            }

            // classificar a ordem pela distância
            std::sort(windows.begin(), windows.end(), TransparentSorted::compare);

            // desenha todas as janelas respeitando as ordens
            for(auto &window : windows) {
                window_shader->use();

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, window.position);
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

                window_shader->setMatrix4fv("model", glm::value_ptr(model));

                mesh_window->draw(window_shader);
            }
        }

        glEnable(GL_CULL_FACE);

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    delete mesh_gress;
    delete mesh_window;
    delete mesh_floor;

    delete grass_shader;
    delete window_shader;
    delete floor_shader;

    delete camera;

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
