#ifndef UBO_HPP
#define UBO_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class UBO
{
    public:
        UBO(const std::string &name, unsigned int _size, GLuint _binding);
        virtual ~UBO();

        void bind();
        void unbind();

        void UBOSubBuffer(const void *_buffer, unsigned int _offset, unsigned int _size);

        GLuint getUBO() { return this->_ubo; }
        GLuint getBinding() { return this->_binding; }
        std::string getName() { return this->_name; }

    protected:
        GLuint _ubo;
        GLuint _binding;
        std::string _name;

    private:
};

#endif // UBO_HPP
