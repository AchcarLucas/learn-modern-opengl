#version 330 core

layout (location = 0) in vec3 iPosition;

layout (std140) uniform Light {
    mat4 space;
} light;

uniform mat4 model;

void main()
{
	gl_Position = light.space * model * vec4(iPosition, 1.0);
}
