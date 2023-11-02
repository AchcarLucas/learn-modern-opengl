#include "model.hpp"

Model::Model(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    std::cout << "LOAD::MODEL <" << path << ">" << std::endl;

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

Model::~Model()
{
    // destruir todas as meshes e textures quando o model for destruido
    for(auto &mesh : this->meshes) {
        delete mesh;
    }

    for(auto &textures : this->textures) {
        delete textures;
    }
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh *Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture2D *> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0]) {
            vertex.tex = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.tex = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int i = 0; i < face.mNumIndices; i++) {
            indices.push_back(face.mIndices[i]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture2D *> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    // 2. specular maps
    std::vector<Texture2D *> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    // 3. normal maps
    std::vector<Texture2D *> normal_maps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::NORMAL);
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
    // 4. height maps
    std::vector<Texture2D *> height_maps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::AMBIENT_OCCLUSION);
    textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    return new Mesh(vertices, indices, textures);
}

std::vector<Texture2D*> Model::loadMaterialTextures(aiMaterial *material, aiTextureType ai_type, TextureType texture_type)
{
    std::vector<Texture2D *> _textures;

    for(unsigned int i = 0; i < material->GetTextureCount(ai_type); ++i) {
        aiString str;
        material->GetTexture(ai_type, i, &str);

        bool skip = false;
        for(unsigned int i = 0; i < textures.size(); i++) {
            if(!std::strcmp(textures[i]->getFile().c_str(), str.data)) {
                // mantém a textura já carregada e pula a etapa de carregamento de texturas
                textures.push_back(textures[i]);
                // considerando apenas um tipo de material por vez
                skip = true;
                break;
            }
        }

        // se a textura já existe, ela já foi carregada e associada, não precisamos recarregar
        if(!skip) {
            _textures.push_back(new Texture2D(str.data, this->directory, texture_type));
        }
    }

    return _textures;
}

void Model::draw(Shader *shader)
{
    for(auto &mesh : this->meshes) {
        mesh->draw(shader);
    }
}
