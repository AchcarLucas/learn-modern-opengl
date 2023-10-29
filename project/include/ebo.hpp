#ifndef EBO_HPP
#define EBO_HPP

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class EBO
{
    public:
        EBO();
        virtual ~EBO();

        void bind();
        void EBOBuffer(unsigned int *, unsigned int, unsigned int);

        GLuint getEBO();

    protected:
        GLuint _ebo;

    private:
};

#endif // EBO_HPP
