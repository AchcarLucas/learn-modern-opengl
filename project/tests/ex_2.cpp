#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int run_002(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    Texture2D *texture_0 = new Texture2D("resources/textures/container.jpg");
    Texture2D *texture_1 = new Texture2D("resources/textures/awesomeface.png", TextureType::ALBEDO);

    Shader *shader = new Shader("glsl/test_2_vertex_shader.vs", "glsl/test_2_fragment_shader.fs");
    SObject *quad = createQuad();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader->use();
    shader->setMatrix4fv("model", glm::value_ptr(model));
    shader->setMatrix4fv("view", glm::value_ptr(view));
    shader->setMatrix4fv("projection", glm::value_ptr(projection));
    shader->setInt("texture0", 0);
    shader->setInt("texture1", 1);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        shader->use();

        texture_0->bind(GL_TEXTURE0);
        texture_1->bind(GL_TEXTURE1);

        quad->_vao->bind();
        quad->_vbo->bind();
        quad->_ebo->bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete texture_0;
    delete texture_1;

    delete shader;
    delete quad;

    glfwTerminate();
    return 0;
}
