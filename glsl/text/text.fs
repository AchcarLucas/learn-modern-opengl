#version 330 core

out vec4 FragColor;

in VS_DATA {
	vec2 tex;
} vs_in;

uniform sampler2D text_sampler;
uniform vec3 text_color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text_sampler, vs_in.tex).r);
    FragColor = vec4(text_color, 1.0) * sampled;
}  