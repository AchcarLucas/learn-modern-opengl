#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;
layout (location = 4) in vec3 iOffset;

out VS_DATA {
    vec3 color;
} vs_out;

void main()
{
    vs_out.color = iColor;
    vec3 pos = iPosition * (gl_InstanceID / 100.0);
    gl_Position = vec4(pos + iOffset, 1.0);
}
