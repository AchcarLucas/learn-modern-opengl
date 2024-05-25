#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

out VS_DATA {
    vec2 tex;
} vs_out;

void main()
{
    vs_out.tex = iTexCoord;
    gl_Position = vec4(iPosition.xy, 0.0, 1.0); 
}