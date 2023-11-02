#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

inline void processInput(GLFWwindow *, float);
inline void mouseCallback(GLFWwindow*, double, double);
inline void scrollCallback(GLFWwindow*, double, double);

// camera class
inline Camera *camera = new Camera();

// timing
inline float delta_time = 0.0f;	// time between current frame and last frame
inline float last_frame = 0.0f;

int run_007(const int width, const int height)
{
    float _WIDTH = width;
    float _HEIGHT = height;

    _stbi_set_flip_vertically_on_load(true);

    Shader *shader_cube = new Shader("glsl/ex_7/ex_7_vertex_shader.vs", "glsl/ex_7/ex_7_fragment_shader.fs");
    Shader *shader_light = new Shader("glsl/ex_7/light_vertex_shader.vs", "glsl/ex_7/light_fragment_shader.fs");

    SObject *cube = createCube();
    SObject *light = createLight();

    glm::vec4 light_position(1.2f, 1.0f, 2.0f, 0.0f);

    Texture2D *texture_0 = new Texture2D("resources/textures/container.jpg");
    Texture2D *texture_1 = new Texture2D("resources/textures/awesomeface.png", true);

    shader_cube->use();
    shader_cube->setInt("texture0", 0);
    shader_cube->setInt("texture1", 1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_DEPTH_TEST);

    float angle_rotate = 0.0f;

    while(!glfwWindowShouldClose(window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camera->getCamPos(), camera->getCamPos() + camera->getCamFront(), camera->getUpVector());

        angle_rotate += 1.0f;

        {
            shader_cube->use();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(-45.0f + angle_rotate), glm::vec3(1.0f, 1.0f, 1.0f));

            glm::vec3 object_color = glm::vec3(1.0f, 0.5f, 0.31f);
            glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

            shader_cube->setUniform3fv("objectColor", glm::value_ptr(object_color));
            shader_cube->setUniform3fv("lightColor",  glm::value_ptr(light_color));
            shader_cube->setUniform3fv("lightPos", glm::value_ptr(light_position));
            shader_cube->setUniform3fv("viewPos", glm::value_ptr(camera->getCamPos()));

            shader_cube->setMatrix4fv("model", glm::value_ptr(model));
            shader_cube->setMatrix4fv("view", glm::value_ptr(view));
            shader_cube->setMatrix4fv("projection", glm::value_ptr(projection));

            texture_0->bind(GL_TEXTURE0);
            texture_1->bind(GL_TEXTURE1);

            cube->_vao->bind();
            cube->_ebo->bind();

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        {
            shader_light->use();

            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(light_position));
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

            shader_light->setMatrix4fv("model", glm::value_ptr(model));
            shader_light->setMatrix4fv("view", glm::value_ptr(view));
            shader_light->setMatrix4fv("projection", glm::value_ptr(projection));

            light->_vao->bind();
            light->_ebo->bind();

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete texture_0;
    delete texture_1;

    delete shader_cube;
    delete shader_light;

    delete cube;
    delete light;

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
