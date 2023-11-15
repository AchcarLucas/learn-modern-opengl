#ifndef RBO_HPP
#define RBO_HPP

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.hpp"

class RBO
{
    public:
        RBO(const int width, const int height, const GLenum gl_internalformat);
        RBO(const int width, const int height, const GLenum gl_internalformat, unsigned int multisample);
        virtual ~RBO();

        void bind();
        void unbind();

        GLuint getRBO() { return this->_rbo; }

    protected:
        GLuint _rbo;

    private:
};

#endif // RBO_HPP
