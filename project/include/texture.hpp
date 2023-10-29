#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "image.hpp"

class Texture2D
{
    public:
        Texture2D(std::string, bool alpha_channel = false);
        virtual ~Texture2D();

        void bind(GLenum);

    protected:
        GLuint texture;
        unsigned char *image;

    private:
};

#endif // TEXTURE_HPP
