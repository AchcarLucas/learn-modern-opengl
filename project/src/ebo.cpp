#include "ebo.hpp"

EBO::EBO()
{
    glGenBuffers(1, &this->_ebo);
}

EBO::~EBO()
{
    glDeleteBuffers(1, &this->_ebo);
}

void EBO::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
}

void EBO::EBOBuffer(const void *_buffer, unsigned int _size, unsigned int _usage)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _buffer, _usage);
}

GLuint EBO::getEBO()
{
    return this->_ebo;
}
