#version 330 core

out vec4 FragColor;

in vec3 vertex_color;
in vec2 tex_coord;

// texture samplers
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	FragColor = mix(texture(texture0, tex_coord), texture(texture1, tex_coord), 0.2);
}