#include "ubo.hpp"

UBO::UBO(const std::string &name, unsigned int _size, GLuint _binding)
{
    glGenBuffers(1, &this->_ubo);

    this->_binding = _binding;
    this->_name = name;

    this->bind();
    // cria o buffer e initializa com zero
    glBufferData(GL_UNIFORM_BUFFER, _size, NULL, GL_STATIC_DRAW);

    this->unbind();
    // define o range do buffer total do buffer uniforme
    glBindBufferRange(GL_UNIFORM_BUFFER, this->_binding, this->_ubo, 0, _size);
}

UBO::~UBO()
{
    glDeleteBuffers(1, &this->_ubo);
}

void UBO::bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, this->_ubo);
}

void UBO::unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::UBOSubBuffer(const void *_buffer, unsigned int _offset, unsigned int _size)
{
    this->bind();
    glBufferSubData(GL_UNIFORM_BUFFER, _offset, _size, _buffer);
    this->unbind();
}
