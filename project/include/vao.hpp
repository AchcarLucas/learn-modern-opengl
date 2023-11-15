#ifndef VAO_HPP
#define VAO_HPP

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vbo.hpp"
#include "ebo.hpp"

class VAO
{
    public:
        VAO();
        virtual ~VAO();

        void bind();
        void unbind();

        GLuint getVAO();
    protected:
        GLuint _vao;

    private:
};

#endif // VAO_HPP
