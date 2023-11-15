#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;

out VS_DATA {
    vec3 color;
} vs_out;

uniform vec3 pos_offset[100];

void main()
{
    vs_out.color = iColor;
    vec3 offset = pos_offset[gl_InstanceID];
    gl_Position = vec4(iPosition + offset, 1.0);
}
