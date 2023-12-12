#version 330 core

out vec4 FragColor;

in VS_DATA {
	vec2 tex;
} vs_in;

uniform sampler2D text_sampler;
uniform vec3 text_color;

void main()
{    
    float glyph_shape = texture(text_sampler, vs_in.tex).r;

    if (glyph_shape < 0.5)
        discard;

    FragColor = vec4(text_color, 1.0);
}  