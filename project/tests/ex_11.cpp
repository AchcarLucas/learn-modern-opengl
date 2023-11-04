#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

inline void processInput(GLFWwindow *, float);
inline void mouseCallback(GLFWwindow*, double, double);
inline void scrollCallback(GLFWwindow*, double, double);

// camera class
inline Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));

// timing
inline float delta_time = 0.0f;	// time between current frame and last frame
inline float last_frame = 0.0f;

inline std::vector<Vertex> ex_11_vertices = {
    Vertex(glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f))
};

inline std::vector<GLuint> ex_11_indices = {
    0, 1, 3,
    1, 2, 3
};

int run_011(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    Shader *shader = new Shader("glsl/first_vertex_shader.vs", "glsl/first_fragment_shader.fs");

    std::vector<Texture2D*> textures;

    textures.push_back(new Texture2D("resources/textures/container.jpg"));
    textures.push_back(new Texture2D("resources/textures/awesomeface.png", TextureType::ALBEDO));

    Mesh *mesh = new Mesh(ex_11_vertices, ex_11_indices, textures);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        mesh->draw(shader);

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    delete mesh;
    delete shader;
    delete camera;

    for (auto& texture : textures) {
        delete texture;
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float delta_time)
{
    camera->processInput(window, delta_time);
}

void mouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    camera->mouseCallback(window, x_pos, y_pos);
}

void scrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    camera->scrollCallback(window, x_offset, y_offset);
}
