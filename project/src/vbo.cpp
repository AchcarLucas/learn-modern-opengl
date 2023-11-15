#include "vbo.hpp"

VBO::VBO()
{
    glGenBuffers(1, &this->_vbo);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &this->_vbo);
}

void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
}

void VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::VBOBuffer(const void *_buffer, unsigned int _size, unsigned int usage)
{
    glBufferData(GL_ARRAY_BUFFER, _size, _buffer, usage);
}

void VBO::VBOBufferDivisor(GLuint _index, GLint _size, GLenum _type, GLsizei stride, const void *pointer)
{
    const GLuint divisor = 1;
    glEnableVertexAttribArray(_index);
    this->bind();
    glVertexAttribPointer(_index, _size, _type, GL_FALSE, stride, pointer);
    this->unbind();
    glVertexAttribDivisor(_index, divisor);
}

GLuint VBO::getVBO()
{
    return this->_vbo;
}

