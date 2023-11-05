#ifndef RBO_HPP
#define RBO_HPP

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class RBO
{
    public:
        RBO(const int width, const int height, const GLenum gl_internalformat);
        virtual ~RBO();

        void bind();
        GLuint getRBO() { return this->_rbo; }

    protected:
        GLuint _rbo;

    private:
};

#endif // RBO_HPP
