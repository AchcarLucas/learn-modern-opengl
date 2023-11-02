#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

Mesh::~Mesh()
{
    //dtor
}

void Mesh::Draw(Shader *shader)
{
    // vamos criar um array de contadores para cada tipo de textura
    std::map<TextureType, unsigned int> textures;

    // todos os tipos vão iniciar com zero
    for (auto& texture : this->textures) {
        textures[texture->getType()] = 0;
    }

    unsigned int i_texture = 0;

    for (auto& texture : this->textures) {
        // obtemos o tipo de textura
        TextureType type = texture->getType();
        // vamos mapear ele (obter o nome) e já criar o seu index do tipo
        std::string texture_name = textureTypeMap[type] + "_" + std::to_string(++textures[type]);
        // será a estrutura interna do nosso shader
        shader->setInt(("material." + texture_name).c_str(), i_texture);
        // vamos já criar o bind para a nossa textura
        texture->bind(GL_TEXTURE0 + (++i_texture));
    }

    shader->use();
    this->vao.bind();
    this->ebo.bind();
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    this->vao.bind();

    this->vbo.bind();
    this->vbo.VBOBuffer(&this->vertices[0], this->vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

    this->ebo.bind();
    this->ebo.EBOBuffer(&this->indices[0], this->indices.size() * sizeof(GLuint), GL_STATIC_DRAW);

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // vertex texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
