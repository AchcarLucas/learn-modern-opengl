#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures, VERTEX_TYPE vt)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMeshBTN(vt);
    setupMesh(vt);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures, std::vector<TextureCube*> textures_cube, VERTEX_TYPE vt)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->textures_cube = textures_cube;

    setupMeshBTN(vt);
    setupMesh(vt);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture2D*> textures, VERTEX_TYPE vt)
{
    this->vertices = vertices;
    this->textures = textures;

    setupMeshBTN(vt);
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

    for (auto& texture : this->textures_cube) {
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
    if(vt == ATTRIB_PCN || vt == ATTRIB_PNT || vt == ATTRIB_PNTBT) {
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(index++);
    }

    // vertex texs
    if(vt == ATTRIB_PNT || vt == ATTRIB_PT || vt == ATTRIB_PNTBT) {
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
        glEnableVertexAttribArray(index++);
    }

    // para ter tangent e bitangent tem que ter tex_coord
    if((vt == ATTRIB_PNT || vt == ATTRIB_PT || vt == ATTRIB_PNTBT)) {
        // vertex tangent
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
        glEnableVertexAttribArray(index++);

        // vertex bitangent
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
        glEnableVertexAttribArray(index++);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// POS_1, POS_2, POS_3
glm::vec3 Mesh::calcTangent(glm::vec3 p_1, glm::vec3 p_2, glm::vec3 p_3, glm::vec2 uv_1, glm::vec2 uv_2, glm::vec2 uv_3)
{
    glm::vec3 tangent;

    glm::vec3 edge_1 = p_2 - p_1;
    glm::vec3 edge_2 = p_3 - p_1;

    glm::vec2 delta_uv_1 = uv_2 - uv_1;
    glm::vec2 delta_uv_2 = uv_3 - uv_1;

    float f = 1.0f / (delta_uv_1.x * delta_uv_2.y - delta_uv_2.x * delta_uv_1.y);

    tangent.x = f * (delta_uv_2.y * edge_1.x - delta_uv_1.y * edge_2.x);
    tangent.y = f * (delta_uv_2.y * edge_1.y - delta_uv_1.y * edge_2.y);
    tangent.z = f * (delta_uv_2.y * edge_1.z - delta_uv_1.y * edge_2.z);

    return tangent;
}

glm::vec3 Mesh::calcBitangent(glm::vec3 p_1, glm::vec3 p_2, glm::vec3 p_3, glm::vec2 uv_1, glm::vec2 uv_2, glm::vec2 uv_3)
{
    glm::vec3 bitangent;

    glm::vec3 edge_1 = p_2 - p_1;
    glm::vec3 edge_2 = p_3 - p_1;

    glm::vec2 delta_uv_1 = uv_2 - uv_1;
    glm::vec2 delta_uv_2 = uv_3 - uv_1;

    float f = 1.0f / (delta_uv_1.x * delta_uv_2.y - delta_uv_2.x * delta_uv_1.y);

    bitangent.x = f * (-delta_uv_2.x * edge_1.x + delta_uv_1.x * edge_2.x);
    bitangent.y = f * (-delta_uv_2.x * edge_1.y + delta_uv_1.x * edge_2.y);
    bitangent.z = f * (-delta_uv_2.x * edge_1.z + delta_uv_1.x * edge_2.z);

    return bitangent;
}

void Mesh::setupMeshBTN(VERTEX_TYPE vt)
{
    if(!((vt == ATTRIB_PNT || vt == ATTRIB_PT) && indices.empty()))
        return;

    for(unsigned i = 0; i < vertices.size(); i += 3) {
        glm::vec3 p_1 = vertices[i].position;
        glm::vec2 uv_1 = vertices[i].tex;

        glm::vec3 p_2 = vertices[i + 1].position;
        glm::vec2 uv_2 = vertices[i + 1].tex;

        glm::vec3 p_3 = vertices[i + 2].position;
        glm::vec2 uv_3 = vertices[i + 2].tex;

        glm::vec3 tangent = calcTangent(p_1, p_2, p_3, uv_1, uv_2, uv_3);
        glm::vec3 bitangent = calcBitangent(p_1, p_2, p_3, uv_1, uv_2, uv_3);

        vertices[i].tangent = tangent;
        vertices[i + 1].tangent = tangent;
        vertices[i + 2].tangent = tangent;

        vertices[i].bitangent = bitangent;
        vertices[i + 1].bitangent = bitangent;
        vertices[i + 2].bitangent = bitangent;

        std::cout << "tangent: " << glm::to_string(tangent) << "\nbitangent: " << glm::to_string(bitangent) << std::endl;
    }
}
