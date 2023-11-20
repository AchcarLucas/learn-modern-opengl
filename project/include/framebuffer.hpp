#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "rbo.hpp"
#include "texture.hpp"

class FrameBuffer
{
    public:
        FrameBuffer(const int width, const int height);
        FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment, TextureType type = TextureType::FRAMEBUFFER);
        FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment, unsigned int multisample);
        virtual ~FrameBuffer();

        void bind(GLenum target = GL_FRAMEBUFFER);
        void unbind();

        Texture2D *getTexture2D() { return framebuffer_tex; }
        RBO *getRBO() { return this->rbo; }

        int getWidth() { return this->width; }
        int getHeight() { return this->height; }

    protected:
        GLuint fbo;
        Texture2D *framebuffer_tex;
        RBO *rbo;

        int width, height;

    private:
};

#endif // FRAMEBUFFER_HPP
