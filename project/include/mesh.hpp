#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.hpp"
#include "shader.hpp"

#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex;

    Vertex() { };

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex)
    {
        this->position = position;
        this->normal = normal;
        this->tex = tex;
    }
};

class Mesh
{
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures);
        virtual ~Mesh() { };

        void draw(Shader *shader);

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture2D*> textures;

    protected:
        VAO vao;
        VBO vbo;
        EBO ebo;

    private:
        void setupMesh();
};

#endif // MESH_HPP
