#version 330 core
out vec4 FragColor;

in VS_DATA {
	vec4 frag_position;
	vec3 normal;
	vec2 tex;
} vs_in;

uniform sampler2D _texture;

void main()
{
  FragColor = vec4(vec3(texture(_texture, vs_in.tex)), 1.0);
}