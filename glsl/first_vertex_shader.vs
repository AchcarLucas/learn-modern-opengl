#version 330 core

layout (location = 0) in vec3 iPosition;

out vec3 outColor;

void main()
{
    gl_Position = vec4(iPosition, 1.0);
}
