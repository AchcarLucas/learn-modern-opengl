#include "object.hpp"

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
    SObject *object = new SObject();

    VAO *_vao = new VAO();
    VBO *_vbo = new VBO();
    EBO *_ebo = new EBO();

    object->_vao = _vao;
    object->_ebo = _ebo;
    object->_vbo = _vbo;

    _vao->bind();

    _vbo->bind();
    _vbo->VBOBuffer(quad_vertices, sizeof(quad_vertices), GL_STATIC_DRAW);

    _ebo->bind();
    _ebo->EBOBuffer(quad_indices, sizeof(quad_indices), GL_STATIC_DRAW);

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
    SObject *object = new SObject();

    VAO *_vao = new VAO();
    VBO *_vbo = new VBO();
    EBO *_ebo = new EBO();

    object->_vao = _vao;
    object->_ebo = _ebo;
    object->_vbo = _vbo;

    _vao->bind();

    _vbo->bind();
    _vbo->VBOBuffer(triangle_vertices, sizeof(triangle_vertices), GL_STATIC_DRAW);

    _ebo->bind();
    _ebo->EBOBuffer(triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW);

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
