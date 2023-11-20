#ifndef MODEL_HPP
#define MODEL_HPP

#include "mesh.hpp"
#include "texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

struct ModelTransform {
    glm::mat4 model = glm::mat4(1.0f);

    ModelTransform() { };

    ModelTransform(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) {
        positionModel(position);
        scaleModel(scale);
    };

    ModelTransform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotate, float angle) {
        positionModel(position);
        scaleModel(scale);
        rotateModel(rotate, angle);
    };

    glm::mat4 positionModel(glm::vec3 position) {
        model = glm::translate(model, position);
        return model;
    }

    glm::mat4 scaleModel(glm::vec3 scale) {
        model = glm::scale(model, scale);
        return model;
    }

    glm::mat4 rotateModel(glm::vec3 rotate, float angle) {
        model = glm::rotate(model, glm::degrees(angle), rotate);
        return model;
    }

    glm::mat4 getModel() {
        return model;
    }
};

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
