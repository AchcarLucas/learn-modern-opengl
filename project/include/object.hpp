#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "vao.hpp"

struct SObject
{
    VAO *_vao;
    VBO *_vbo;
    EBO *_ebo;
};

SObject *createTriangle();
SObject *createQuad();

#endif // OBJECT_HPP
