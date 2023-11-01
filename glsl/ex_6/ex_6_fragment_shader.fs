#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 fragNormal;
in vec3 fragPosition;

void main()
{
	vec3 norm = normalize(fragNormal);
	vec3 light_dir = normalize(lightPos - fragPosition);

	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff;

    vec3 result = (ambientStrength + diff) * lightColor * objectColor;
	FragColor = vec4(result, 1.0);
}