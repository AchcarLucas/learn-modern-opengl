#version 330 core
out vec4 FragColor;

in VS_DATA {
    vec2 tex;
} vs_in;

uniform sampler2D screen_texture;

void main()
{
    /*
    vec3 col = texture(screen_texture, vs_in.tex).rgb;
    float grayscale = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
    FragColor = vec4(vec3(grayscale), 1.0);
    */

    FragColor = texture(screen_texture, vs_in.tex);
} 