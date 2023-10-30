#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int run_006(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    Shader *shader_cube = new Shader("glsl/ex_6_color/ex_6_vertex_shader.vs", "glsl/ex_6_color/ex_6_fragment_shader.fs");
    Shader *shader_light = new Shader("glsl/ex_6_color/light_vertex_shader.vs", "glsl/ex_6_color/light_fragment_shader.fs");

    SObject *cube = createCube();
    SObject *light = createLight();

    glm::vec3 light_position(1.2f, 1.0f, 2.0f);

    Texture2D *texture_0 = new Texture2D("resources/textures/container.jpg");
    Texture2D *texture_1 = new Texture2D("resources/textures/awesomeface.png", true);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

    shader_cube->use();
    shader_cube->setInt("texture0", 0);
    shader_cube->setInt("texture1", 1);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        {
            shader_cube->use();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 1.0f));

            glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
            glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

            shader_cube->setUniform3fv("objectColor", glm::value_ptr(objectColor));
            shader_cube->setUniform3fv("lightColor",  glm::value_ptr(lightColor));

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
            model = glm::translate(model, light_position);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

            shader_light->setMatrix4fv("model", glm::value_ptr(model));
            shader_light->setMatrix4fv("view", glm::value_ptr(view));
            shader_light->setMatrix4fv("projection", glm::value_ptr(projection));

            light->_vao->bind();
            light->_ebo->bind();

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        processInput(window);

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
