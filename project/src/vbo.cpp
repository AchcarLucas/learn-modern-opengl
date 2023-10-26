#include "vbo.hpp"

VBO::VBO()
{
    glGenBuffers(1, &this->_vbo);
}


void VBO::setVBOBuffer(float *_buffer, unsigned int _size, unsigned int usage)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
    glBufferData(GL_ARRAY_BUFFER, _size, _buffer, usage);
}

