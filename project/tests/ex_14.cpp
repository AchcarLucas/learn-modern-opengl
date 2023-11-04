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

inline void processInput(GLFWwindow *, float);
inline void mouseCallback(GLFWwindow*, double, double);
inline void scrollCallback(GLFWwindow*, double, double);

inline Camera *camera = new Camera();

// timing
inline float delta_time = 0.0f;	// time between current frame and last frame
inline float last_frame = 0.0f;

int run_014(const int width, const int height)
{
    float _WIDTH = width;
    float _HEIGHT = height;

    camera->setCamPos(glm::vec3(0.0f, 0.0f, 10.0f));

    //_stbi_set_flip_vertically_on_load(true);

    Shader *shader = new Shader("glsl/ex_14/model_loading.vs", "glsl/ex_14/model_loading.fs");
    Shader *outlining_shader = new Shader("glsl/ex_14/outlining.vs", "glsl/ex_14/outlining.fs");

    Model *model_obj = new Model("./resources/obj/house/cottage_fbx.fbx");

    std::vector<Texture2D*> textures;

    textures.push_back(new Texture2D("resources/obj/house/cottage_diffuse.png", TextureType::DIFFUSE));
    textures.push_back(new Texture2D("resources/obj/house/cottage_normal.png", TextureType::NORMAL));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // depth buffer test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // stencil buffer test
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glStencilMask(0x00);

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glm::mat4 projection = camera->getPerspectiveMatrix(_WIDTH, _HEIGHT);
        glm::mat4 view = camera->getViewMatrix();

        /////////////////////////////////////////////



        // draw

        // habilita a escrita no buffer de stencil
        glStencilMask(0xFF);
        // onde desenhar, adiciona 1 ao buffer do stencil
        glStencilFunc(GL_ALWAYS, 1, 0xFF);

        {
            shader->use();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(3.0f, 1.0f, 2.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            shader->setMatrix4fv("model", glm::value_ptr(model));
            shader->setMatrix4fv("projection", glm::value_ptr(projection));
            shader->setMatrix4fv("view", glm::value_ptr(view));

            shader->setInt("material.diffuse_1", 0);
            shader->setInt("material.normal_1", 1);

            textures[0]->bind(GL_TEXTURE0);
            textures[1]->bind(GL_TEXTURE1);

            model_obj->draw(shader);
        }


        /////////////////////////////////////////////

        // outlining

        // desabilita a escrita no buffer de stencil
        glStencilMask(0x00);
        // só desenha em locais onde o buffer é diferente de 1 (onde não foi desenhado anteriormente
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        // desabilita a escrita no buffer de profundidade
        glDepthMask(GL_FALSE);

        {
            outlining_shader->use();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(3.01f, 1.01f, 2.01f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            shader->setMatrix4fv("model", glm::value_ptr(model));
            shader->setMatrix4fv("projection", glm::value_ptr(projection));
            shader->setMatrix4fv("view", glm::value_ptr(view));

            model_obj->draw(outlining_shader);
        }

        // habilita a escrita no stencil novamente
        glStencilMask(0xFF);
        // sempre escrever 1 no buffer (ao renderizar algo)
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        // habilita novamente a escrita no buffer de profundidade
        glDepthMask(GL_TRUE);

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete model_obj;
    delete shader;
    delete outlining_shader;
    delete camera;

    for(auto &texture : textures) {
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
