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
inline Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));

// timing
inline float delta_time = 0.0f;	// time between current frame and last frame
inline float last_frame = 0.0f;

int run_009(const int width, const int height)
{
    float _WIDTH = width;
    float _HEIGHT = height;

    _stbi_set_flip_vertically_on_load(true);

    //Shader *shader_cube = new Shader("glsl/ex_9/global_light.vs", "glsl/ex_9/global_light.fs");
    Shader *shader_cube = new Shader("glsl/ex_9/point_light.vs", "glsl/ex_9/point_light.fs");
    Shader *shader_light = new Shader("glsl/ex_9/light_vertex_shader.vs", "glsl/ex_9/light_fragment_shader.fs");

    SObject *cube = createCube();
    SObject *light = createLight();

    Texture2D *texture_0 = new Texture2D("resources/textures/container2.png", true);
    Texture2D *texture_1 = new Texture2D("resources/textures/container2_specular.png", true);

    shader_cube->use();
    shader_cube->setInt("material.diffuse", 0);
    shader_cube->setInt("material.specular", 1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_DEPTH_TEST);

    glm::vec4 light_vector(1.2f, 1.0f, 2.0f, 0.0f);

    glm::vec3 cube_positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    while(!glfwWindowShouldClose(window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camera->getCamPos(), camera->getCamPos() + camera->getCamFront(), camera->getUpVector());

        {
            shader_cube->use();

            glm::vec3 light_ambient(0.2f, 0.2f, 0.2f);
            glm::vec3 light_diffuse(0.5f, 0.5f, 0.5f);
            glm::vec3 light_specular(1.0f, 1.0f, 1.0f);

            float constant = 1.0f;
            float linear = 0.09f;
            float quadratic = 0.032f;

            float material_shininess = 64.0f;

            float i_time = glfwGetTime();

            shader_cube->setUniform1fv("iTime", &i_time);

            shader_cube->setUniform3fv("viewPos", glm::value_ptr(camera->getCamPos()));

            shader_cube->setUniform4fv("light.vector", glm::value_ptr(light_vector));
            shader_cube->setUniform3fv("light.ambient", glm::value_ptr(light_ambient));
            shader_cube->setUniform3fv("light.diffuse", glm::value_ptr(light_diffuse));
            shader_cube->setUniform3fv("light.specular", glm::value_ptr(light_specular));
            shader_cube->setUniform1fv("light.constant", &constant);
            shader_cube->setUniform1fv("light.linear", &linear);
            shader_cube->setUniform1fv("light.quadratic", &quadratic);

            shader_cube->setUniform1fv("material.shininess", &material_shininess);

            shader_cube->setMatrix4fv("view", glm::value_ptr(view));
            shader_cube->setMatrix4fv("projection", glm::value_ptr(projection));

            texture_0->bind(GL_TEXTURE0);
            texture_1->bind(GL_TEXTURE1);

            cube->_vao->bind();
            cube->_ebo->bind();

            for(unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cube_positions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                shader_cube->setMatrix4fv("model", glm::value_ptr(model));
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

        }

        {
            shader_light->use();

            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(light_vector));
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
