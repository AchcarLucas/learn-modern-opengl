#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse_1;
    sampler2D normal_1;
    sampler2D specular_1;
};

uniform Material material;

in VS_DATA {
    vec2 tex;
} vs_in;

void main()
{
    FragColor = texture(material.diffuse_1, vs_in.tex);
}