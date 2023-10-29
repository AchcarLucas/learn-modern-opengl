#include "vao.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &this->_vao);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &this->_vao);
}

void VAO::bind()
{
    glBindVertexArray(this->_vao);
}

GLuint VAO::getVAO()
{
    return this->_vao;
}
