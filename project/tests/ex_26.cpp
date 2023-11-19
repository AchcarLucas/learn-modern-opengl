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

static std::vector<Vertex> mesh_ex_26_with_indices = {
    Vertex(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    Vertex(glm::vec3(0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    Vertex(glm::vec3(-0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    Vertex(glm::vec3(-0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)),
};

static std::vector<Vertex> mesh_ex_26_without_indices = {
    Vertex(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    Vertex(glm::vec3(0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    Vertex(glm::vec3(-0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    Vertex(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    Vertex(glm::vec3(-0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    Vertex(glm::vec3(-0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)),
};

static std::vector<unsigned int> indices_mesh_ex_26 = {
    0, 1, 2,
    0, 2, 3
};

int run_026(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    Shader *shader = new Shader("glsl/ex_26/quad.vs", "glsl/ex_26/quad.fs");
    Mesh *mesh = new Mesh(mesh_ex_26_with_indices, indices_mesh_ex_26, std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PC);
    //Mesh *mesh = new Mesh(mesh_ex_26_without_indices, std::vector<GLuint>(), std::vector<Texture2D*>(), VERTEX_TYPE::ATTRIB_PC);

    glm::vec3 translations[100];

    {
        int index = 0;
        float offset = 0.1f;

        for(int y = -10; y < 10; y += 2) {
            for(int x = -10; x < 10; x += 2) {
                glm::vec3 translation;
                translation.x = (float)x / 10.0f + offset;
                translation.y = (float)y / 10.0f + offset;
                translation.z = 0.0f;
                translations[index++] = translation;
            }
        }
    }

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        shader->use();

        for(unsigned int i = 0; i < 100; i++) {
            shader->setUniform3fv("pos_offset[" + std::to_string(i) + "]", glm::value_ptr(translations[i]));
        }

        mesh->draw(shader, 100);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader;
    delete mesh;

    glfwTerminate();
    return 0;
}
