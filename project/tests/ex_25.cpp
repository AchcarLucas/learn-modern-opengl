#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "ubo.hpp"

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

int run_025(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    //Shader *shader_object = new Shader("glsl/ex_25/explode.vs", "glsl/ex_25/explode.fs", "glsl/ex_25/explode.gs");
    Shader *shader_object = new Shader("glsl/ex_25/model_loading.vs", "glsl/ex_25/model_loading.fs");
    //Shader *shader_normal = new Shader("glsl/ex_25/normal.vs", "glsl/ex_25/normal.fs", "glsl/ex_25/normal.gs");
    Shader *shader_normal = new Shader("glsl/ex_25/normal_center.vs", "glsl/ex_25/normal_center.fs", "glsl/ex_25/normal_center.gs");

    Model *my_model = new Model("./resources/obj/backpack/backpack.obj");

    glm::mat4 projection = camera->getPerspectiveMatrix(width, height);
    glm::mat4 view = camera->getViewMatrix();

    UBO *ubo_matrices = new UBO("Matrices", 2 * sizeof(glm::mat4), 0);
    UBO *ubo_global = new UBO("Global", 1 * sizeof(float), 1);

    ubo_matrices->UBOSubBuffer(glm::value_ptr(projection), 0, sizeof(glm::mat4));
    ubo_matrices->UBOSubBuffer(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));

    shader_object->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
    shader_object->setUniformBlockBinding(ubo_global->getName(), ubo_global->getBinding());

    shader_normal->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
    shader_normal->setUniformBlockBinding(ubo_global->getName(), ubo_global->getBinding());

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // atualização das uniform buffer
        glm::mat4 view = camera->getViewMatrix();
        ubo_matrices->UBOSubBuffer(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));
        ubo_global->UBOSubBuffer(&current_frame, 0, sizeof(float));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        // draw object
        {
            shader_object->use();
            shader_object->setMatrix4fv("model", glm::value_ptr(model));
            my_model->draw(shader_object);
        }

        // draw normal
        {
            shader_normal->use();
            shader_normal->setMatrix4fv("model", glm::value_ptr(model));
            my_model->draw(shader_normal);
        }

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader_object;
    delete shader_normal;

    delete my_model;

    delete ubo_matrices;
    delete ubo_global;

    delete camera;

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


