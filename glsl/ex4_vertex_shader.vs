#version 330 core
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;
layout (location = 2) in vec2 iTexCoord;

out vec3 vertex_color;
out vec2 tex_coord;

uniform mat4 transform;

void main()
{
    vertex_color = iColor;
    tex_coord = vec2(iTexCoord.x, iTexCoord.y);
	gl_Position = transform * vec4(iPosition, 1.0);
}