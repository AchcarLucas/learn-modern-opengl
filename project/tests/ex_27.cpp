#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "vbo.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

inline std::vector<Vertex> mesh_ex_26_with_indices = {
    Vertex(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    Vertex(glm::vec3(0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    Vertex(glm::vec3(-0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    Vertex(glm::vec3(-0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)),
};

inline std::vector<Vertex> mesh_ex_26_without_indices = {
    Vertex(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    Vertex(glm::vec3(0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    Vertex(glm::vec3(-0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    Vertex(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    Vertex(glm::vec3(-0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    Vertex(glm::vec3(-0.05f, 0.05f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)),
};

inline std::vector<unsigned int> indices_mesh_ex_26 = {
    0, 1, 2,
    0, 2, 3
};

int run_027(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    Shader *shader = new Shader("glsl/ex_27/quad_m.vs", "glsl/ex_27/quad_m.fs");
    //Shader *shader = new Shader("glsl/ex_27/quad.vs", "glsl/ex_27/quad.fs");
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

    VBO *vbo_array = new VBO();

    {
        // vamos primeiro fazer o bind no nosso VAO da mesh
        mesh->getVAO()->bind();

        // agora vamos adicionar os dados de posição em uma VBOBufferDivisor
        vbo_array->bind();
        vbo_array->VBOBuffer(glm::value_ptr(translations[0]), sizeof(glm::vec3) * 100, GL_STATIC_DRAW);
        vbo_array->unbind();
        vbo_array->VBOBufferDivisor(4, 3, GL_FLOAT, sizeof(glm::vec3));
    }

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        shader->use();
        mesh->draw(shader, 100);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader;
    delete mesh;
    delete vbo_array;

    glfwTerminate();
    return 0;
}
