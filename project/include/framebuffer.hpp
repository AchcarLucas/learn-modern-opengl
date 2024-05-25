#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "rbo.hpp"
#include "texture.hpp"

class AttachmentFrameBuffer
{
    public:
        AttachmentFrameBuffer(int width,
                    int height,
                    TextureType type,
                    GLenum gl_internalformat,
                    GLenum gl_format,
                    GLenum variable_type
        )
        {
            this->width = width;
            this->height = height;
            this->type = type;
            this->gl_internalformat = gl_internalformat;
            this->gl_format = gl_format;
            this->variable_type = variable_type;
        }

        int width;
        int height;
        TextureType type;
        GLenum gl_internalformat;
        GLenum gl_format;
        GLenum variable_type;
};

template <typename T>
class FrameBuffer
{
    public:
        FrameBuffer(std::vector<AttachmentFrameBuffer> attachment_frame_buffer);
        FrameBuffer(const int width, const int height, unsigned num_attachment = 1);
        FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment, TextureType type = TextureType::FRAMEBUFFER, unsigned num_attachment = 1);
        FrameBuffer(const int width, const int height, const GLenum gl_internalformat, const GLenum gl_attachment, unsigned int multisample, unsigned num_attachment = 1);
        virtual ~FrameBuffer();

        void bind(GLenum target = GL_FRAMEBUFFER);
        void unbind();

        T *getTexture(unsigned index = 0) { return framebuffer_tex[index]; }
        RBO *getRBO() { return this->rbo; }

        int getWidth() { return this->width; }
        int getHeight() { return this->height; }

    protected:
        std::vector<T *> framebuffer_tex;
        GLuint fbo;
        RBO *rbo;

        int width, height;

    private:
};

#endif // FRAMEBUFFER_HPP
