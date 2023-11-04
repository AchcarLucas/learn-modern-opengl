#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "image.hpp"

enum TextureType {
    DIFFUSE = 1,
    SPECULAR,
    EMISSIVE,
    ALBEDO,
    AMBIENT,
    AMBIENT_OCCLUSION,
    NORMAL,
    OPACITY,
    SHININESS,
    METALLIC,
    ROUGHNESS,
    BUMP,
    GLOSSY,
    DISPLACEMENT,
    REFLECTION,
    LIGHTMAP,
    HEIGHT
};

typedef std::map<TextureType, std::string> TextureTypeMap;

inline TextureTypeMap textureTypeMap = {
    {TextureType::DIFFUSE, "diffuse"},
    {TextureType::SPECULAR, "specular"},
    {TextureType::EMISSIVE, "emissive"},
    {TextureType::ALBEDO, "albedo"},
    {TextureType::AMBIENT_OCCLUSION, "ambient_occlusion"},
    {TextureType::AMBIENT, "ambient"},
    {TextureType::NORMAL, "normal"},
    {TextureType::OPACITY, "opacity"},
    {TextureType::SHININESS, "shininess"},
    {TextureType::METALLIC, "metallic"},
    {TextureType::ROUGHNESS, "roughness"},
    {TextureType::BUMP, "bump"},
    {TextureType::GLOSSY, "glossy"},
    {TextureType::DISPLACEMENT, "displacement"},
    {TextureType::REFLECTION, "reflection"},
    {TextureType::LIGHTMAP, "lightmap"},
    {TextureType::HEIGHT, "height"},
};

class Texture2D
{
    public:
        Texture2D(const std::string file, const TextureType type = TextureType::ALBEDO);
        virtual ~Texture2D();

        void bind(GLenum);

        TextureType getType() { return this->type; }
        std::string getFile() { return this->file; }
        GLenum getFormat() { return this->format; }

    protected:
        GLuint texture;
        TextureType type;
        GLenum format;

    private:
        std::string file;
};

#endif // TEXTURE_HPP
