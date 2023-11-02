#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragPos;

struct Light {
    vec3 position;
    vec3 direction;
    float inner_cutoff;
    float outer_cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;

    sampler2D texture;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emission;
    float shininess;
};

uniform float iTime;

uniform vec3 viewPos;
uniform vec2 viewPort;

uniform Material material;
uniform Light light;

void main()
{
	vec3 norm = normalize(fragNormal);
	vec3 light_dir = normalize(light.position.xyz - fragPos);

	// ambiente
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.inner_cutoff - light.outer_cutoff;

    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    // flashlight texture
    vec2 fragCoord = gl_FragCoord.xy / viewPort * vec2(1.0, -1.0);
    intensity = intensity * length(vec3(texture(light.texture, fragCoord)));

    // suggestions!
    //float intensity = smoothstep(0.0, 1.0, (theta - light.outer_cutoff) / epsilon);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

    // diffuso
    vec3 diffuse = max(dot(norm, light_dir), 0.0f) * light.diffuse * vec3(texture(material.diffuse, texCoord));

    // specular
    vec3 specular_map = vec3(texture(material.specular, texCoord));
    vec3 view_dir = normalize(viewPos - fragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * light.specular * specular_map;

    vec3 result = ambient + (diffuse + specular) * intensity;

    FragColor = vec4(result, 1.0f);
}
