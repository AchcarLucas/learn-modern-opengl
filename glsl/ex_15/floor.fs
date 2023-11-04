#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D albedo_1;
};

uniform Material material;

void main()
{    
    vec4 texColor = texture(material.albedo_1, TexCoords);
    FragColor = texColor;
}