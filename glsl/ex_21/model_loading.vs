#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
} matrices;

out vec2 TexCoords;

uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = matrices.projection * matrices.view * model * vec4(aPos, 1.0);
}
