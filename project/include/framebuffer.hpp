#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "rbo.hpp"
#include "texture.hpp"

template <typename T>
class FrameBuffer
{
    public:
        FrameBuffer(const int width, const int height, unsigned num_attachment = 1);
        FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment, TextureType type = TextureType::FRAMEBUFFER, unsigned num_attachment = 1);
        FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment, unsigned int multisample, unsigned num_attachment = 1);
        virtual ~FrameBuffer();

        void bind(GLenum target = GL_FRAMEBUFFER);
        void unbind();

        T *getTexture() { return framebuffer_tex; }
        RBO *getRBO() { return this->rbo; }

        int getWidth() { return this->width; }
        int getHeight() { return this->height; }

    protected:
        T *framebuffer_tex;
        GLuint fbo;
        RBO *rbo;

        int width, height;

    private:
};

#endif // FRAMEBUFFER_HPP
