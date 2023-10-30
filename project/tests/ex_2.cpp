
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

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        processInput(window);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
