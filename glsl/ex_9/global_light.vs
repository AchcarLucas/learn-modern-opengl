#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;
layout (location = 2) in vec2 iTexCoord;
layout (location = 3) in vec3 iNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec3 fragNormal;
out vec3 fragPos;

void main()
{
	texCoord = vec2(iTexCoord.x, iTexCoord.y);
	fragPos = vec3(model * vec4(iPosition, 1.0));
	// calcular a normal na CPU (eficiencia!)
	fragNormal = mat3(transpose(inverse(model))) * iNormal;
	gl_Position = projection * view * model * vec4(iPosition, 1.0);
}
