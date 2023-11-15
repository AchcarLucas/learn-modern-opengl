#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTex;
layout (location = 3) in mat4 iModel;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
} matrices;

layout (std140) uniform Global {
    float time;
} global;

out VS_DATA {
    vec2 tex;
} vs_out;

void main()
{
    vs_out.tex = iTex;
    gl_Position = matrices.projection * matrices.view * iModel * vec4(iPosition, 1.0);
}
