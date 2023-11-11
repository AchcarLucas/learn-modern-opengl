#version 330 core
out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 camera_pos;

struct Material {
    sampler2D diffuse_1;
    sampler2D normal_1;
    sampler2D specular_1;
};

uniform Material material;

in VS_DATA {
    vec2 tex_coords;
    vec3 normal;
    vec3 position;
} vs_in;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 _incident = normalize(vs_in.position - camera_pos);
    /*
        vec3 _reflect = reflect(_incident, normalize(normal));
        FragColor = vec4(texture(skybox, _reflect).rgb, 1.0f);
    */

    vec3 _refract = refract(_incident, normalize(vs_in.normal), ratio);
    FragColor = vec4(texture(skybox, _refract).rgb, 1.0f);
    
}