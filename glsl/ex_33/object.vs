#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

out VS_DATA {
	vec4 frag_model_position;
	vec4 frag_shadow_position;
	vec3 normal;
	vec2 tex;
} vs_out;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
} matrices;

struct Light {
  vec3 position;

  mat4 view;

  float near_plane;
  float far_plane;
};

uniform Light light;

uniform mat4 model;

void main()
{
	vs_out.frag_model_position = model * vec4(iPosition, 1.0);
	vs_out.frag_shadow_position = light.view * vs_out.frag_model_position;

	vs_out.normal = mat3(transpose(inverse(model))) * iNormal;
	vs_out.tex = iTexCoord;

	gl_Position = matrices.projection * matrices.view * vs_out.frag_model_position;
}
