#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static void processInput(GLFWwindow *, float);
static void mouseCallback(GLFWwindow*, double, double);
static void scrollCallback(GLFWwindow*, double, double);

static Camera *camera = new Camera();

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

int run_013(const int width, const int height)
{
    float _WIDTH = width;
    float _HEIGHT = height;

    camera->setCamPos(glm::vec3(0.0f, 0.0f, 10.0f));

    //_stbi_set_flip_vertically_on_load(true);

    Shader *shader = new Shader("glsl/ex_13/model_loading.vs", "glsl/ex_13/model_loading.fs");
    Model *model_obj = new Model("./resources/obj/house/cottage_fbx.fbx");

    std::vector<Texture2D*> textures;

    textures.push_back(new Texture2D("resources/obj/house/cottage_diffuse.png", TextureType::DIFFUSE));
    textures.push_back(new Texture2D("resources/obj/house/cottage_normal.png", TextureType::NORMAL));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_DEPTH_TEST);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        shader->use();

        glm::mat4 projection = camera->getPerspectiveMatrix(_WIDTH, _HEIGHT);
        glm::mat4 view = camera->getViewMatrix();

        shader->setMatrix4fv("projection", glm::value_ptr(projection));
        shader->setMatrix4fv("view", glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 2.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        shader->setMatrix4fv("model", glm::value_ptr(model));

        shader->setInt("material.diffuse_1", 0);
        shader->setInt("material.normal_1", 1);

        textures[0]->bind(GL_TEXTURE0);
        textures[1]->bind(GL_TEXTURE1);

        model_obj->draw(shader);

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete model_obj;
    delete shader;
    delete camera;

    for(auto &texture : textures) {
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
