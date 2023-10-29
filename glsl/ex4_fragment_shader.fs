#version 330 core

out vec4 FragColor;

in vec3 vertex_color;
in vec2 tex_coord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, tex_coord), texture(texture2, tex_coord), 0.2);
}