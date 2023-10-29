#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

void changeFrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #if __APPLE__
        #include <TargetConditionals.h>
        #if TARGET_OS_MAC
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
    #endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL with GLFW", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, changeFrameBufferSizeCallback);

    Texture2D *texture_1 = new Texture2D("resources/textures/container.jpg");
    Texture2D *texture_2 = new Texture2D("resources/textures/wall.jpg");

    Shader *shader_1 = new Shader("glsl/first_vertex_shader.vs", "glsl/first_fragment_shader.fs");
    Shader *shader_2 = new Shader("glsl/interpolate_vertex_shader.vs", "glsl/interpolate_fragment_shader.fs");
    Shader *shader_3 = new Shader("glsl/texture_vertex_shader.vs", "glsl/texture_fragment_shader.fs");

    shader_3->use();
    shader_3->setInt("texture1", 0);
    shader_3->setInt("texture2", 1);

    SObject *quad = createQuad();
    SObject *triangle = createTriangle();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        texture_1->bind(GL_TEXTURE1);
        texture_2->bind(GL_TEXTURE2);

        shader_3->use();
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

    delete triangle;
    delete quad;

    glfwTerminate();
    return 0;
}
