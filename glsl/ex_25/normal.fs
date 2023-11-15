#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse_1;
    sampler2D normal_1;
    sampler2D specular_1;
};

uniform Material material;

in GS_DATA {
    vec2 tex;
    vec3 normal;
} gs_in;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}