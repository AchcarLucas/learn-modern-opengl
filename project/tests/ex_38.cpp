#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
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
static Camera *camera = new Camera();

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

int run_038(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    Shader *shader = new Shader("glsl/first_vertex_shader.vs", "glsl/first_fragment_shader.fs");
    Shader *shader_text = new Shader("glsl/text/text.vs", "glsl/text/text.fs");

    SObject *quad = createQuad();

    RenderText *renderText = new RenderText("fonts/arial.ttf", width, height, 0, 48);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        shader->use();
        quad->_vao->bind();
        quad->_ebo->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        renderText->draw(shader_text, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        renderText->draw(shader_text, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader;
    delete quad;

    delete renderText;

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
