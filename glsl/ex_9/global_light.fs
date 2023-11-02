#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragPos;

struct Light {
    vec4 vector;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emission;
    float shininess;
};

uniform float iTime;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	vec3 norm = normalize(fragNormal);
	vec3 light_dir = (light.vector.w == 0.0f) ? normalize(-light.vector.xyz) : normalize(light.vector.xyz - fragPos);

	// ambiente
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
	
	// diffuso
	vec3 diffuse = max(dot(norm, light_dir), 0.0f) * light.diffuse * vec3(texture(material.diffuse, texCoord));

	// specular
	vec3 specular_map = vec3(texture(material.specular, texCoord));
	vec3 view_dir = normalize(viewPos - fragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * light.specular * specular_map;

	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0f);
}