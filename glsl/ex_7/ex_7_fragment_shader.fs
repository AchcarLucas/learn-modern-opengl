#version 330 core

out vec4 FragColor;

in vec3 fragNormal;
in vec3 fragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	vec3 norm = normalize(fragNormal);

	// ambiente
	vec3 ambient = light.ambient * material.ambient;

	// diffuso
	vec3 light_dir = normalize(light.position - fragPos);
	vec3 diffuse = max(dot(norm, light_dir), 0.0f) * light.diffuse * material.diffuse;

	// specular
	vec3 view_dir = normalize(viewPos - fragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * light.specular * material.specular;

	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0f);
}