#ifndef MODEL_HPP
#define MODEL_HPP

#include "mesh.hpp"
#include "texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

class Model
{
    public:
        Model(std::string path);
        virtual ~Model();

        void draw(Shader *shader, GLuint instances = 1);

        std::vector<Mesh *> getMeshes() { return meshes; }

    protected:
        std::vector<Mesh *> meshes;
        std::vector<Texture2D *> textures;
        std::string directory;

    private:
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh *processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture2D*> loadMaterialTextures(aiMaterial *material, aiTextureType ai_type, TextureType texture_type);
};

#endif // MODEL_HPP
