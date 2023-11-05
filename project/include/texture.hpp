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
    HEIGHT,
    FRAMEBUFFER
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
    {TextureType::FRAMEBUFFER, "framebuffer"},
};

class Texture2D
{
    public:
        Texture2D(const std::string file, const TextureType type = TextureType::ALBEDO);
        Texture2D(const int width, const int height, const TextureType type = TextureType::FRAMEBUFFER);
        virtual ~Texture2D();

        void bind(GLenum);

        TextureType getType() { return this->type; }
        std::string getFile() { return this->file; }
        GLenum getFormat() { return this->format; }

        unsigned int getWidth() { return this->width; }
        unsigned int getHeight() { return this->height; }

        GLuint getGenTexture() { return texture; }
    protected:
        GLuint texture;
        TextureType type;
        GLenum format;
        unsigned int width, height;

    private:
        std::string file;
};

#endif // TEXTURE_HPP
