#version 330 core

layout (location = 0) in vec3 iPosition;

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
	gl_Position = light.view * model * vec4(iPosition, 1.0);
}
