#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;

out vec3 outColor;

void main()
{
    outColor = iColor;
    gl_Position = vec4(iPosition, 1.0);
}
