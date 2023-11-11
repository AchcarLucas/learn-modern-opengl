#version 330 core
out vec4 FragColor;

in vec2 tex_coords;
in vec3 normal;
in vec3 position;

struct Material {
    sampler2D diffuse_1;
    sampler2D normal_1;
    sampler2D specular_1;
};

uniform Material material;
uniform samplerCube skybox;
uniform vec3 camera_pos;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 _incident = normalize(position - camera_pos);
    /*
        vec3 _reflect = reflect(_incident, normalize(normal));
        FragColor = vec4(texture(skybox, _reflect).rgb, 1.0f);
    */

    vec3 _refract = refract(_incident, normalize(normal), ratio);
    FragColor = vec4(texture(skybox, _refract).rgb, 1.0f);
    
}