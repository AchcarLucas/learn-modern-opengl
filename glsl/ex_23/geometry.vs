#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;

out VS_DATA {
    vec3 color;
} vs_out;

void main()
{
    vs_out.color = iColor;
    gl_Position = vec4(iPosition, 1.0);
}
