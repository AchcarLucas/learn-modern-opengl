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
        void unbind();

        void VBOBuffer(const void *, unsigned int, unsigned int);
        void VBOBufferDivisor(GLuint _index, GLint _size, GLenum _type, GLsizei stride, const void *pointer = (void*)0);

        GLuint getVBO();

    protected:
        GLuint _vbo;

    private:
};

#endif // VBO_HPP
