#version 330 core

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 fragNormal;
in vec3 fragPos;

void main()
{
	float ambient_strength = 0.1;
	float specular_strength = 0.8;

	vec3 norm = normalize(fragNormal);

	// ambiente
	float ambient = ambient_strength;

	// diffuso
	vec3 light_dir = normalize(lightPos - fragPos);
	float diffuse = max(dot(norm, light_dir), 0.0f);

	// specular
	vec3 view_dir = normalize(viewPos - fragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float specular = pow(max(dot(view_dir, reflect_dir), 0.0f), 32) * specular_strength;

	vec3 result = (ambient + diffuse + specular) * lightColor * objectColor;

	FragColor = vec4(result, 1.0f);
}