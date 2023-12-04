#version 330 core
out vec4 FragColor;

in VS_DATA {
	vec4 frag_position;
	vec3 normal;
	vec2 tex;
} vs_in;

layout (std140) uniform Camera {
    vec3 position;
} camera;

struct Light {
  vec3 position;

  mat4 views[6];

  float near_plane;
  float far_plane;
};

uniform Light light;
uniform samplerCube depth_cubemap;

void main()
{
	vec3 r = vec3(vs_in.tex - 0.5, 1.0f);
	float depth = texture(depth_cubemap, r / light.far_plane).r;
    FragColor = vec4(vec3(depth), 1.0);
}