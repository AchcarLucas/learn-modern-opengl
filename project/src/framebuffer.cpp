#include "framebuffer.hpp"

template class FrameBuffer<Texture2D>;
template class FrameBuffer<TextureCube>;

template <typename T>
FrameBuffer<T>::FrameBuffer(const int width, const int height, unsigned num_attachment)
{
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    for(unsigned i = 0; i < num_attachment; ++i) {
        framebuffer_tex = dynamic_cast<T*>(new Texture2D(width, height));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, framebuffer_tex->getGenTexture(), 0);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        delete this;
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

template <typename T>
FrameBuffer<T>::FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment, TextureType type, unsigned num_attachment)
{
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    switch(type) {
        case TextureType::FRAMEBUFFER_DEPTH_MAPPING:
            framebuffer_tex = dynamic_cast<T*>(new Texture2D(width, height, type, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, framebuffer_tex->getGenTexture(), 0);
            break;
        case TextureType::FRAMEBUFFER_DEPTH_CUBEMAP:
            framebuffer_tex = dynamic_cast<T*>(new TextureCube(width, height));
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, framebuffer_tex->getGenTexture(), 0);
            break;
        default:
            for(unsigned i = 0; i < num_attachment; ++i) {
                framebuffer_tex = dynamic_cast<T*>(new Texture2D(width, height, type));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, framebuffer_tex->getGenTexture(), 0);
            }
            break;
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        delete this;
        return;
    }

    switch(type) {
        case TextureType::FRAMEBUFFER_DEPTH_MAPPING:
        case TextureType::FRAMEBUFFER_DEPTH_CUBEMAP:
            break;
        case TextureType::FRAMEBUFFER:
        default:
            this->rbo = new RBO(width, height, gl_internalformat);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, gl_attachment, GL_RENDERBUFFER, this->rbo->getRBO());
            break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

template <typename T>
FrameBuffer<T>::FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment, unsigned int multisample, unsigned num_attachment)
{
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    this->rbo = new RBO(width, height, gl_internalformat, multisample);

    for(unsigned i = 0; i < num_attachment; ++i) {
        framebuffer_tex = dynamic_cast<T*>(new Texture2D(width, height, multisample));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, framebuffer_tex->getGenTexture(), 0);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER_MULTISAMPLE:: Framebuffer is not complete!" << std::endl;
        delete this;
        return;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, gl_attachment, GL_RENDERBUFFER, this->rbo->getRBO());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

template <typename T>
FrameBuffer<T>::~FrameBuffer()
{
    delete framebuffer_tex;
    delete rbo;

    glDeleteFramebuffers(1, &this->fbo);
}

template <typename T>
void FrameBuffer<T>::bind(GLenum target)
{
    glBindFramebuffer(target, this->fbo);
}

template <typename T>
void FrameBuffer<T>::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
