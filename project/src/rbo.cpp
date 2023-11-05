#include "rbo.hpp"

RBO::RBO(const int width, const int height, const GLenum gl_internalformat)
{
    glGenRenderbuffers(1, &this->_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, this->_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, gl_internalformat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RBO::~RBO()
{
    glDeleteRenderbuffers(1, &this->_rbo);
}

void RBO::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, this->_rbo);
}
