#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int run_022(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    Shader *shader = new Shader("glsl/ex_22/geometry.vs", "glsl/ex_22/geometry.fs", "glsl/ex_22/geometry.gs");
    SObject *quad = createQuad();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        shader->use();

        quad->_vao->bind();
        quad->_ebo->bind();
        glDrawArrays(GL_POINTS, 0, 4);
        /*
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        */

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader;
    delete quad;

    glfwTerminate();
    return 0;
}
