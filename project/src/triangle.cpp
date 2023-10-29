#include "object.hpp"

float triangle_vertices[] =
{
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
};

VBO *createTriangle()
{
    VBO *_vbo = new VBO();
    _vbo->VBOBuffer(triangle_vertices, sizeof(triangle_vertices), GL_STATIC_DRAW);
    return _vbo;
}
