#include "framebuffer.hpp"

FrameBuffer::FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment)
{
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    framebuffer_tex = new Texture2D(width, height, TextureType::FRAMEBUFFER);

    this->rbo = new RBO(width, height, gl_internalformat);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_tex->getGenTexture(), 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        delete this;
        return;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, gl_attachment, GL_RENDERBUFFER, this->rbo->getRBO());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    delete framebuffer_tex;
    delete rbo;

    glDeleteFramebuffers(1, &this->fbo);
}

void FrameBuffer::bind(GLenum target)
{
    glBindFramebuffer(target, this->fbo);
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
