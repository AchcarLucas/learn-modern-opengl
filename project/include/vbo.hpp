#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VBO
{
    public:
        VBO();
        void setVBOBuffer(float *, unsigned int, unsigned int);

    protected:
        unsigned int _vbo;

    private:
};

#endif // VBO_H
