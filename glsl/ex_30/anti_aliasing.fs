#version 330 core
out vec4 FragColor;

in VS_DATA {
    vec2 tex;
} vs_in;

uniform sampler2D screen_texture;

void main()
{
    vec3 col = texture(screen_texture, vs_in.tex).rgb;
    FragColor = vec4(vec3(col), 1.0);
} 