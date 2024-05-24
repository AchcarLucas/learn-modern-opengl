#version 330 core
out vec4 FragColor;

in VS_DATA {
    vec2 tex;
} vs_in;

uniform float gamma;
uniform float exposure;
uniform bool hdr_enabled;
uniform sampler2D screen_texture;

vec3 gammaCorrection(vec3 color)
{
    return pow(color, vec3(1.0 / gamma));
}

void main()
{
    if(hdr_enabled) {
        vec3 col = texture(screen_texture, vs_in.tex).rgb;
        vec3 mapped = vec3(1.0) - exp(-col * exposure);
        FragColor = vec4(gammaCorrection(mapped.xyz), 1.0);
    } else {
        vec3 col = texture(screen_texture, vs_in.tex).rgb;
        FragColor = vec4(gammaCorrection(col.xyz), 1.0);
    }
} 