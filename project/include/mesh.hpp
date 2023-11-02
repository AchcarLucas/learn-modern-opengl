#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex;
};

class Mesh
{
    public:
        Mesh();
        virtual ~Mesh();

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture2D> textures;

    protected:

    private:
};

#endif // MESH_HPP
