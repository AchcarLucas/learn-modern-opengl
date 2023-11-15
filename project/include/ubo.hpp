#ifndef UBO_HPP
#define UBO_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class UBO
{
    public:
        UBO(unsigned int _size, GLuint _binding);
        virtual ~UBO();

        void bind();
        void unbind();

        void UBOSubBuffer(const void *_buffer, unsigned int _offset, unsigned int _size);

        GLuint getUBO() { return this->_ubo; }

    protected:
        GLuint _ubo;
        GLuint _binding;

    private:
};

#endif // UBO_HPP
