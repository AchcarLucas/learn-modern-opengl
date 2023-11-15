#include "rbo.hpp"

RBO::RBO(const int width, const int height, const GLenum gl_internalformat)
{
    glGenRenderbuffers(1, &this->_rbo);
    this->bind();
    glRenderbufferStorage(GL_RENDERBUFFER, gl_internalformat, width, height);
    this->unbind();
}

RBO::RBO(const int width, const int height, const GLenum gl_internalformat, unsigned int multisample)
{
    glGenRenderbuffers(1, &this->_rbo);
    this->bind();
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample, gl_internalformat, width, height);
    this->unbind();
}

RBO::~RBO()
{
    glDeleteRenderbuffers(1, &this->_rbo);
}

void RBO::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, this->_rbo);
}

void RBO::unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
