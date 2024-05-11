#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;
layout (location = 3) in vec3 iTangent;
layout (location = 4) in vec3 iBitangent;

out VS_DATA {
	vec3 position;
	vec2 tex;
	mat3 TBN;
} vs_out;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
} matrices;

uniform mat4 model;

void main()
{
	vs_out.position = vec3(model * vec4(iPosition, 1.0));

	vec3 T = mat3(transpose(inverse(model))) * iTangent;
	vec3 B = mat3(transpose(inverse(model))) * iBitangent;
	vec3 N = mat3(transpose(inverse(model))) * iNormal;
	
	vs_out.TBN = mat3(T, B, N);

	vs_out.tex = iTexCoord;

	gl_Position = matrices.projection * matrices.view * model * vec4(iPosition, 1.0);
}
