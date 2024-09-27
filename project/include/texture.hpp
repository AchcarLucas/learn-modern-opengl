#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>
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
    BUFFER,
    FRAMEBUFFER,
    FRAMEBUFFER_DEPTH_MAPPING,
    FRAMEBUFFER_DEPTH_CUBEMAP,
    FRAMEBUFFER_CUBEMAP,
    FRAMEBUFFER_MULTISAMPLE
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
    {TextureType::BUFFER, "buffer"},
    {TextureType::FRAMEBUFFER, "framebuffer"},
    {TextureType::FRAMEBUFFER_DEPTH_MAPPING, "depth"},
    {TextureType::FRAMEBUFFER_DEPTH_CUBEMAP, "depth_cubemap"},
    {TextureType::FRAMEBUFFER_CUBEMAP, "cubemap"},
    {TextureType::FRAMEBUFFER_MULTISAMPLE, "multisample"}
};

class TextureBase
{
    public:
        virtual ~TextureBase();

        TextureType getType() { return this->type; }
        GLenum getFormat() { return this->format; }

        unsigned int getWidth() { return this->width; }
        unsigned int getHeight() { return this->height; }

        virtual void bind(GLenum) = 0;

        GLuint getGenTexture() { return texture; }

    protected:
        GLuint texture;
        TextureType type;
        GLenum format;
        unsigned int width, height;
};

class Texture2D : public TextureBase
{
    public:
        Texture2D(const std::string file, const TextureType type = TextureType::ALBEDO, bool flip = true, const GLenum gl_format = GL_NONE, const GLenum texture_wrap_s = GL_REPEAT, const GLenum texture_wrap_t = GL_REPEAT, GLenum variable_type = GL_UNSIGNED_BYTE);
        Texture2D(const int width, const int height, const TextureType type = TextureType::FRAMEBUFFER, const GLenum gl_internalformat = GL_RGB, const GLenum gl_format = GL_RGB, GLenum variable_type = GL_UNSIGNED_BYTE);
        Texture2D(const int width, const int height, unsigned int multisample);

        void bind(GLenum);

        std::string getFile() { return this->file; }

    private:
        std::string file;
};

class TextureCube : public TextureBase
{
    public:
        TextureCube(std::vector<std::string> files, bool flip = true);
        TextureCube(const int width, const int height);

        void bind(GLenum);

        std::vector<std::string> getFiles() { return this->files; }

    private:
        std::vector<std::string> files;
};

class TextureBuffer : public TextureBase
{
    public:
        TextureBuffer(const int width,
                             const int height,
                             const void *buffer,
                             const GLenum type = GL_FLOAT,
                             const GLint format = GL_RGB,
                             const GLint internal_format = GL_RGBA32F,
                             const GLenum min_filter = GL_NEAREST,
                             const GLenum mag_filter = GL_NEAREST,
                             const GLenum wrap_s = GL_REPEAT,
                             const GLenum wrap_t = GL_REPEAT);

    void bind(GLenum);
};

#endif // TEXTURE_HPP
