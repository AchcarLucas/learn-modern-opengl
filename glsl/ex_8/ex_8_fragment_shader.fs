#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragPos;

struct Light {
    vec3 position;
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

	// ambiente
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	// diffuso
	vec3 light_dir = normalize(light.position - fragPos);
	vec3 diffuse = max(dot(norm, light_dir), 0.0f) * light.diffuse * vec3(texture(material.diffuse, texCoord));

	// specular
	vec3 specular_map = vec3(texture(material.specular, texCoord));
	vec3 view_dir = normalize(viewPos - fragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * light.specular * specular_map;

	// emission (with effect sin)
	vec3 emission = vec3(texture(material.emission, texCoord + 0.045f + vec2(0.0, iTime * 0.75))) * (sin(iTime) * 0.5 + 0.5) * 2.0f;
	emission = emission * step(vec3(1.0f), vec3(1.0f) - specular_map);
	

	vec3 result = (ambient + diffuse + specular + emission);
	FragColor = vec4(result, 1.0f);
}