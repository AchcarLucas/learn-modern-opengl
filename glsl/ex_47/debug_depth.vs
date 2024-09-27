#version 330 core
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

out VS_DATA {
	vec4 frag_position;
	vec3 normal;
	vec2 tex;
} vs_out;

void main()
{
	vs_out.frag_position = vec4(iPosition, 1.0);
	vs_out.normal = iNormal;
    vs_out.tex = iTexCoord;
    gl_Position = vec4(iPosition, 1.0);
}