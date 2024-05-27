#version 330 core

layout (location = 0) in vec3 iPosition;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(iPosition, 1.0);
}
