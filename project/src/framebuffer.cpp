#include "framebuffer.hpp"

template class FrameBuffer<TextureBase>;
template class FrameBuffer<Texture2D>;
template class FrameBuffer<TextureCube>;
template class FrameBuffer<TextureBuffer>;

template <typename T>
FrameBuffer<T>::FrameBuffer(int width, int height, std::vector<AttachmentFrameBuffer> attachment_frame_buffer)
{
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    this->rbo = nullptr;
    this->width = width;
    this->height = height;

    unsigned int attachments[attachment_frame_buffer.size()];

    for(unsigned i = 0; i < attachment_frame_buffer.size(); ++i) {
        T *_texture = dynamic_cast<T*>(new Texture2D(   width,
                                                        height,
                                                        TextureType::FRAMEBUFFER,
                                                        attachment_frame_buffer[i].gl_internalformat,
                                                        attachment_frame_buffer[i].gl_format,
                                                        attachment_frame_buffer[i].variable_type));

        framebuffer_tex.push_back(_texture);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _texture->getGenTexture(), 0);
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glDrawBuffers(attachment_frame_buffer.size(), attachments);

    this->rbo = new RBO(width, height, GL_DEPTH_COMPONENT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->rbo->getRBO());

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        delete this;
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

template <typename T>
FrameBuffer<T>::FrameBuffer(const int width, const int height, unsigned num_attachment)
{
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    this->rbo = nullptr;
    this->width = width;
    this->height = height;

    for(unsigned i = 0; i < num_attachment; ++i) {
        T *_texture = dynamic_cast<T*>(new Texture2D(width, height));
        framebuffer_tex.push_back(_texture);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _texture->getGenTexture(), 0);
    }

    {
        unsigned int attachments[num_attachment];

        for(unsigned index = 0; index < num_attachment; ++index) {
            attachments[index] = GL_COLOR_ATTACHMENT0 + index;
        }

        glDrawBuffers(num_attachment, attachments);
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
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    this->rbo = nullptr;
    this->width = width;
    this->height = height;

    T *_texture;

    switch(type) {
        case TextureType::FRAMEBUFFER_DEPTH_MAPPING:
            _texture = dynamic_cast<T*>(new Texture2D(width, height, type, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT));
            framebuffer_tex.push_back(_texture);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _texture->getGenTexture(), 0);
            break;
        case TextureType::FRAMEBUFFER_DEPTH_CUBEMAP:
            _texture = dynamic_cast<T*>(new TextureCube(width, height));
            framebuffer_tex.push_back(_texture);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texture->getGenTexture(), 0);
            break;
        default:
            for(unsigned i = 0; i < num_attachment; ++i) {
                _texture = dynamic_cast<T*>(new Texture2D(width, height, type));
                framebuffer_tex.push_back(_texture);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _texture->getGenTexture(), 0);
            }

            {
                unsigned int attachments[num_attachment];

                for(unsigned index = 0; index < num_attachment; ++index) {
                    attachments[index] = GL_COLOR_ATTACHMENT0 + index;
                }

                glDrawBuffers(num_attachment, attachments);
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
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    this->rbo = nullptr;
    this->width = width;
    this->height = height;

    this->rbo = new RBO(width, height, gl_internalformat, multisample);

    for(unsigned i = 0; i < num_attachment; ++i) {
        T *_texture = dynamic_cast<T*>(new Texture2D(width, height, multisample));
        framebuffer_tex.push_back(_texture);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, _texture->getGenTexture(), 0);
    }

    {
        unsigned int attachments[num_attachment];

        for(unsigned index = 0; index < num_attachment; ++index) {
            attachments[index] = GL_COLOR_ATTACHMENT0 + index;
        }

        glDrawBuffers(num_attachment, attachments);
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
    if(this->rbo != nullptr)
        delete rbo;

    if(framebuffer_tex.size() > 0) {
        typename std::vector<T *>::iterator it;
        for(it = framebuffer_tex.begin(); it != framebuffer_tex.end(); ++it)
            delete (*it);

        framebuffer_tex.clear();
    }

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
