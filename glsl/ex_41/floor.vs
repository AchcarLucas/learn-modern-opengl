#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;
layout (location = 3) in vec3 iTangent;
layout (location = 4) in vec3 iBitangent;

const int NR_LIGHTS = 16;

out VS_DATA {
	vec3 position;
	vec2 tex;
	mat3 TBN;
	vec3 tangent_light_pos[NR_LIGHTS];
	vec3 tangent_view_pos;
	vec3 tangent_frag_pos;
} vs_out;


layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
} matrices;

layout (std140) uniform Camera {
    vec3 position;
} camera;

struct lightSource {
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  vec3 spot_direction;

  float constant_attenuation;
  float linear_attenuation;
  float quadratic_attenuation;
  float spot_cutoff;
  float spot_exponent;
  
  bool enabled;
};

uniform mat4 model;
uniform lightSource lights[NR_LIGHTS];

void main()
{
	vs_out.position = vec3(model * vec4(iPosition, 1.0));

	vec3 T = mat3(transpose(inverse(model))) * iTangent;
	vec3 B = mat3(transpose(inverse(model))) * iBitangent;
	vec3 N = mat3(transpose(inverse(model))) * iNormal;
	
	vs_out.TBN = mat3(T, B, N);

	vs_out.tex = iTexCoord;

	for(int i = 0; i < NR_LIGHTS; ++i) {
		vs_out.tangent_light_pos[i] = vs_out.TBN * lights[i].position.xyz;
	}

	vs_out.tangent_view_pos = vs_out.TBN * camera.position;
	vs_out.tangent_frag_pos = vs_out.TBN * vs_out.position;

	gl_Position = matrices.projection * matrices.view * model * vec4(iPosition, 1.0);
}
