#ifndef RBO_HPP
#define RBO_HPP

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.hpp"

class RBO
{
    public:
        RBO(const int, const int, const GLenum);
        RBO(const int, const int, const GLenum, unsigned int);
        virtual ~RBO();

        void bind();
        void unbind();

        GLuint getRBO() { return this->_rbo; }

    protected:
        GLuint _rbo;

    private:
};

#endif // RBO_HPP
