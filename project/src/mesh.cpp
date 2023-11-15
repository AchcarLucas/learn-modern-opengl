#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures, VERTEX_TYPE vt)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh(vt);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture2D*> textures, VERTEX_TYPE vt)
{
    this->vertices = vertices;
    this->textures = textures;

    setupMesh(vt);
}

void Mesh::draw(Shader *shader, GLuint instances)
{
    // vamos criar um array de contadores para cada tipo de textura
    std::map<TextureType, unsigned int> textures;

    // todos os tipos vão iniciar com zero
    for (auto& texture : this->textures) {
        textures[texture->getType()] = 0;
    }

    unsigned int i_texture = 0;

    shader->use();

    for (auto& texture : this->textures) {
        // obtemos o tipo de textura
        TextureType type = texture->getType();
        // vamos mapear ele (obter o nome) e já criar o seu index do tipo
        std::string texture_name = textureTypeMap[type] + "_" + std::to_string(++textures[type]);
        // será a estrutura interna do nosso shader
        shader->setInt(("material." + texture_name).c_str(), i_texture);
        // vamos já criar o bind para a nossa textura
        texture->bind(GL_TEXTURE0 + (i_texture++));
    }


    this->vao.bind();

    if(!this->indices.empty()) {
        this->ebo.bind();
        if(instances == 1) {
            glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        } else {
            glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, instances);
        }
    } else {
        if(instances == 1) {
            glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
        } else {
            glDrawArraysInstanced(GL_TRIANGLES, 0, this->vertices.size(), instances);
        }
    }

    glBindVertexArray(0);
}

void Mesh::setupMesh(VERTEX_TYPE vt)
{
    this->vao.bind();

    this->vbo.bind();
    this->vbo.VBOBuffer(&this->vertices[0], this->vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

    if(!this->indices.empty()) {
        this->ebo.bind();
        this->ebo.EBOBuffer(&this->indices[0], this->indices.size() * sizeof(GLuint), GL_STATIC_DRAW);
    }

    unsigned int index = 0;

    // vertex positions
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(index++);

    // vertex colors
    if(vt == ATTRIB_PC || vt == ATTRIB_PCN) {
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(index++);
    }

    // vertex normals
    if(vt == ATTRIB_PCN || vt == ATTRIB_PNT) {
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(index++);
    }

    // vertex texs
    if(vt == ATTRIB_PNT || vt == ATTRIB_PT) {
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
        glEnableVertexAttribArray(index++);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
