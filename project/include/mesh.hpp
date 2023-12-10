#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

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
    glm::vec3 tangent;
    glm::vec3 bitangent;

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

    Vertex(glm::vec3 position, glm::vec2 tex, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent)
    {
        this->position = position;
        this->normal = normal;
        this->tex = tex;
        this->tangent = tangent;
        this->bitangent = bitangent;
    }
};

enum VERTEX_TYPE
{
    ATTRIB_P = 1,   // APENAS POSITION
    ATTRIB_PC,      // POSITION, COLOR
    ATTRIB_PCN,     // POSITION, COLOR, NORMAL
    ATTRIB_PNT,     // POSITION, NORMAL, TEX
    ATTRIB_PT,      // POSITION, TEX
    ATTRIB_PNTBT    // POSITION, NORMAL, TEX, BITANGENT, TANGENT
};

class Mesh
{
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures, VERTEX_TYPE vt = VERTEX_TYPE::ATTRIB_PNT);
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures, std::vector<TextureCube*> textures_cube, VERTEX_TYPE vt = VERTEX_TYPE::ATTRIB_PNT);
        Mesh(std::vector<Vertex> vertices, std::vector<Texture2D*> textures, VERTEX_TYPE vt = VERTEX_TYPE::ATTRIB_PNT);
        virtual ~Mesh() { };

        void draw(Shader *shader, GLuint instances = 1);

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture2D*> textures;
        std::vector<TextureCube*> textures_cube;

        VAO *getVAO() { return &this->vao; }
        VBO *getVBO() { return &this->vbo; }
        EBO *getEBO() { return &this->ebo; }

        glm::vec3 calcTangent(glm::vec3 p_1, glm::vec3 p_2, glm::vec3 p_3, glm::vec2 uv_1, glm::vec2 uv_2, glm::vec2 uv_3);
        glm::vec3 calcBitangent(glm::vec3 p_1, glm::vec3 p_2, glm::vec3 p_3, glm::vec2 uv_1, glm::vec2 uv_2, glm::vec2 uv_3);

    protected:
        VAO vao;
        VBO vbo;
        EBO ebo;

    private:
        void setupMesh(VERTEX_TYPE vt);
        void setupMeshBTN(VERTEX_TYPE vt);
};

#endif // MESH_HPP
