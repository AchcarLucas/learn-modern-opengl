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
    ALBEDO,
    AMBIENT_OCCLUSION,
    NORMAL,
    METALLIC,
    ROUGHNESS,
    BUMP,
    GLOSSY,
    DISPLACEMENT
};

typedef std::map<TextureType, std::string> TextureTypeMap;

inline TextureTypeMap textureTypeMap = {
    {TextureType::DIFFUSE, "texture_diffuse"},
    {TextureType::SPECULAR, "texture_specular"},
    {TextureType::ALBEDO, "texture_albedo"},
    {TextureType::AMBIENT_OCCLUSION, "texture_ambient_occlusion"},
    {TextureType::NORMAL, "texture_normal"},
    {TextureType::METALLIC, "texture_metallic"},
    {TextureType::ROUGHNESS, "texture_roughness"},
    {TextureType::BUMP, "texture_bump"},
    {TextureType::GLOSSY, "texture_glossy"},
    {TextureType::DISPLACEMENT, "texture_displacement"},
};

class Texture2D
{
    public:
        Texture2D(const std::string file, const TextureType type = TextureType::ALBEDO, bool alpha_channel = false);
        Texture2D(const std::string file, const std::string directory, const TextureType type = TextureType::ALBEDO, bool alpha_channel = false);
        virtual ~Texture2D();

        void bind(GLenum);
        TextureType getType();

        std::string getFile() { return file; }
        std::string getDirectory() { return directory; }

    protected:
        GLuint texture;
        TextureType type;

    private:
        std::string file;
        std::string directory;
};

#endif // TEXTURE_HPP
