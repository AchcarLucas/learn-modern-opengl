#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int run_002()
{
    _stbi_set_flip_vertically_on_load(true);

    Shader *shader = new Shader("glsl/test_2_vertex_shader.vs", "glsl/test_2_fragment_shader.fs");
    SObject *quad = createQuad();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        shader->use();
        quad->_vao->bind();
        quad->_ebo->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        processInput(window);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader;
    delete quad;

    glfwTerminate();
    return 0;
}
