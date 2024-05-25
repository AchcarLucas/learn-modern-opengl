#version 330 core
out vec4 FragColor;

in VS_DATA {
    vec2 tex;
} vs_in;

uniform sampler2D render_buffer;
uniform sampler2D blur_buffer;

uniform bool bloom_enabled;

void main()
{
    vec3 render_color = texture(render_buffer, vs_in.tex).rgb;      
    vec3 blur_color = texture(blur_buffer, vs_in.tex).rgb;

    if(bloom_enabled)
        render_color += blur_color; // additive blending

    FragColor = vec4(render_color, 1.0);
} 