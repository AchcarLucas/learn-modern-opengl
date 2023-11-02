#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "image.hpp"

enum TextureType {
    ALBEDO,
    AMBIENT_OCCLUSION,
    NORMAL,
    METALLIC,
    ROUGHNESS,
    BUMP,
    GLOSSY,
    DISPLACEMENT
};
class Texture2D
{
    public:
        Texture2D(std::string, TextureType type = TextureType::ALBEDO, bool alpha_channel = false);
        virtual ~Texture2D();

        void bind(GLenum);

    protected:
        GLuint texture;
        TextureType type;
        unsigned char *image;

    private:
};

#endif // TEXTURE_HPP
