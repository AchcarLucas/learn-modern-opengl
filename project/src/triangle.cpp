#include "triangle.hpp"

float triangle_vertices[] =
{
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

VBO *createTriangle()
{
    VBO *_vbo = new VBO();
    _vbo->setVBOBuffer(triangle_vertices, sizeof(triangle_vertices), GL_STATIC_DRAW);
    return _vbo;
}
