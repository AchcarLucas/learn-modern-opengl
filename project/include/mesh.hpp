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
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 tex;

    Vertex() { };

    Vertex(glm::vec3 position)
    {
        this->position = position;
    }

    Vertex(glm::vec3 position, glm::vec3 color)
    {
        this->position = position;
        this->color = color;
    }

    Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal)
    {
        this->position = position;
        this->color = color;
        this->normal = normal;
    }

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex)
    {
        this->position = position;
        this->normal = normal;
        this->tex = tex;
    }

    Vertex(glm::vec3 position, glm::vec2 tex, glm::vec3 normal)
    {
        this->position = position;
        this->normal = normal;
        this->tex = tex;
    }

    Vertex(glm::vec3 position, glm::vec2 tex)
    {
        this->position = position;
        this->tex = tex;
    }
};

enum VERTEX_TYPE
{
    ATTRIB_P = 1,   // APENAS POSITION
    ATTRIB_PC,      // POSITION, COLOR
    ATTRIB_PCN,     // POSITION, COLOR, NORMAL
    ATTRIB_PNT,     // POSITION, NORMAL, TEX
    ATTRIB_PT       // POSITION, TEX
};

class Mesh
{
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures, VERTEX_TYPE vt = VERTEX_TYPE::ATTRIB_PNT);
        Mesh(std::vector<Vertex> vertices, std::vector<Texture2D*> textures, VERTEX_TYPE vt = VERTEX_TYPE::ATTRIB_PNT);
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
        void setupMesh(VERTEX_TYPE vt);
};

#endif // MESH_HPP
