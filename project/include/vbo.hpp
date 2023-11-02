#ifndef VBO_HPP
#define VBO_HPP

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VBO
{
    public:
        VBO();
        virtual ~VBO();

        void bind();
        void VBOBuffer(const void *, unsigned int, unsigned int);

        GLuint getVBO();

    protected:
        GLuint _vbo;

    private:
};

#endif // VBO_HPP
