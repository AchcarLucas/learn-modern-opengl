#include "object.hpp"

SObject *createObject(float *vertices, unsigned int size_vertices, unsigned int *indices, unsigned int size_indices)
{
    SObject *object = new SObject();

    VAO *_vao = new VAO();
    VBO *_vbo = new VBO();
    EBO *_ebo = new EBO();

    object->_vao = _vao;
    object->_ebo = _ebo;
    object->_vbo = _vbo;

    _vao->bind();

    _vbo->bind();
    _vbo->VBOBuffer(vertices, size_vertices, GL_STATIC_DRAW);

    _ebo->bind();
    _ebo->EBOBuffer(indices, size_indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return object;
}

float quad_vertices[] = {
    // position              color         texture coords
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left
};
unsigned int quad_indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

SObject *createQuad()
{
    return createObject(quad_vertices, sizeof(quad_vertices), quad_indices, sizeof(quad_indices));
}

float triangle_vertices[] =
{
    // position               color          texture coords
    0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
    0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.5f, 1.0f    // top
};

unsigned int triangle_indices[] =
{
    0, 1, 2,   // first triangle
};

SObject *createTriangle()
{
    return createObject(triangle_vertices, sizeof(triangle_vertices), triangle_indices, sizeof(triangle_indices));
}

float cube_vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     +0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     +0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     +0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     +0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     +0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     +0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     +0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     +0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};

unsigned int cube_indices[] =
{
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4,
    8, 9, 10,
    10, 4, 8,
    11, 2, 12,
    12, 13, 11,
    10, 14, 5,
    5, 4, 10,
    3, 2, 11,
    11, 15, 3
};

SObject *createCube()
{
    return createObject(cube_vertices, sizeof(cube_vertices), cube_indices, sizeof(cube_indices));
}
