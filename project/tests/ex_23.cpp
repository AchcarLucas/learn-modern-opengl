#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "mesh.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static std::vector<Vertex> mesh_ex_23 = {
    Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)),
};

static std::vector<unsigned int> indices_mesh_ex_23 = {0, 1, 2, 3};

int run_023(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    Shader *shader = new Shader("glsl/ex_23/geometry.vs", "glsl/ex_23/geometry.fs", "glsl/ex_23/geometry.gs");
    Mesh *mesh = new Mesh(mesh_ex_23, indices_mesh_ex_23, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PC);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        shader->use();
        mesh->getVAO()->bind();
        mesh->getVBO()->bind();
        glDrawArrays(GL_POINTS, 0, 4);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader;
    delete mesh;

    glfwTerminate();
    return 0;
}
