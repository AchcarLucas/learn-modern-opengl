#version 330 core
out vec4 FragColor;

in VS_DATA {
    vec2 tex;
} vs_in;

uniform sampler2D screen_texture;
uniform float gamma = 2.2f;

vec4 gammaCorrection(vec4 color)
{
    return pow(color, 1.0 / vec4(gamma));
}

void main()
{
    vec3 col = texture(screen_texture, vs_in.tex).rgb;
    vec4 color = vec4(vec3(col), 1.0);
    FragColor = gammaCorrection(color);
} 