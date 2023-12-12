#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "rendertext.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static void processInput(GLFWwindow *, float);
static void mouseCallback(GLFWwindow*, double, double);
static void scrollCallback(GLFWwindow*, double, double);

// camera class
static Camera *camera = new Camera(glm::vec3(8.0f, 0.0f, 5.0f), glm::vec3(4, 0, 0));

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

int run_038(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    Shader *shader_text = new Shader("glsl/text/text.vs", "glsl/text/text.fs");

    RenderText *render_text_screen = new RenderText("fonts/arial.ttf", width, height, 0, 48, TextType::DRAW_TO_SCREEN);
    RenderText *render_text_world = new RenderText("fonts/arial.ttf", width, height, 0, 48, TextType::DRAW_TO_WORLD);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)width / (float)height, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glm::mat4 view = glm::lookAt(camera->getCamPos(), camera->getCamPos() + camera->getCamFront(), camera->getUpVector());


        // world text render
        {
            glm::mat4 model(1.0f);
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            model = glm::translate(model, glm::vec3(glm::vec3(0.0, 0.0, 2.0)));
            render_text_world->setMVP(projection * view * model);

            render_text_world->draw(shader_text, "This is sample text into world", 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        }

        // screen text render
        {
            render_text_screen->draw(shader_text, "This is sample text into screen", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
            render_text_screen->draw(shader_text, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        }

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete render_text_screen;
    delete render_text_world;

    delete shader_text;

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
