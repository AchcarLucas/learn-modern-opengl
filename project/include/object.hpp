#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "vao.hpp"

class SObject
{
    public:
        VAO *_vao;
        VBO *_vbo;
        EBO *_ebo;

    ~SObject()
    {
        delete _vao;
        delete _vbo;
        delete _ebo;
    }
};

SObject *createTriangle();
SObject *createQuad();

#endif // OBJECT_HPP
