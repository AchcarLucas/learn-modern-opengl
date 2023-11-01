#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;
layout (location = 2) in vec2 iTexCoord;
layout (location = 2) in vec3 iNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;
out vec3 fragPosition;

void main()
{
	fragNormal = iNormal;
	fragPosition = vec3(model * vec4(iPosition, 1.0));
	gl_Position = projection * view * model * vec4(iPosition, 1.0);
}
