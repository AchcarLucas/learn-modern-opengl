#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "rbo.hpp"
#include "texture.hpp"

class FrameBuffer
{
    public:
        FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment);
        virtual ~FrameBuffer();

        void bind();
        void unbind();

        Texture2D *getTexture2D() { return framebuffer_tex; }
        RBO *getRBO() { return rbo; }

    protected:
        GLuint fbo;
        Texture2D *framebuffer_tex;
        RBO *rbo;

    private:
};

#endif // FRAMEBUFFER_HPP
