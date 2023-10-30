#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int run_001()
{
    _stbi_set_flip_vertically_on_load(true);

    Texture2D *texture_1 = new Texture2D("resources/textures/container.jpg");
    Texture2D *texture_2 = new Texture2D("resources/textures/wall.jpg");
    Texture2D *texture_3 = new Texture2D("resources/textures/awesomeface.png", true);

    Shader *shader_1 = new Shader("glsl/first_vertex_shader.vs", "glsl/first_fragment_shader.fs");
    Shader *shader_2 = new Shader("glsl/interpolate_vertex_shader.vs", "glsl/interpolate_fragment_shader.fs");
    Shader *shader_3 = new Shader("glsl/texture_vertex_shader.vs", "glsl/texture_fragment_shader.fs");
    Shader *shader_4 = new Shader("glsl/ex4_vertex_shader.vs", "glsl/ex4_fragment_shader.fs");

    shader_3->use();
    shader_3->setInt("texture1", 0);
    shader_3->setInt("texture2", 1);

    shader_4->use();
    shader_4->setInt("texture1", 0);
    shader_4->setInt("texture2", 1);

    SObject *quad = createQuad();
    SObject *triangle = createTriangle();

    /*
     * transform test
     */

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 _transform = glm::mat4(1.0f);
    _transform = glm::translate(_transform, glm::vec3(0.5f, 0.5f, 0.0f));
    _transform = glm::rotate(_transform, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));

    shader_4->use();
    shader_4->setMatrix4fv("transform", glm::value_ptr(_transform));

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        texture_1->bind(GL_TEXTURE1);
        texture_3->bind(GL_TEXTURE2);
        shader_4->use();
        quad->_vao->bind();
        quad->_ebo->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /*shader_3->use();
        triangle->_vao->bind();
        triangle->_ebo->bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);*/

        processInput(window);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete texture_1;
    delete texture_2;

    delete shader_1;
    delete shader_2;
    delete shader_3;
    delete shader_4;

    delete triangle;
    delete quad;

    glfwTerminate();
    return 0;
}
