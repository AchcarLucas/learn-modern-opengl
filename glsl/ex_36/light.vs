#version 330 core

layout (location = 0) in vec3 iPosition;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
} matrices;

uniform mat4 model;

void main()
{
	gl_Position = matrices.projection * matrices.view * model * vec4(iPosition, 1.0);
}
