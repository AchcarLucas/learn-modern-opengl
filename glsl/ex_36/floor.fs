#version 330 core

out vec4 FragColor;
out vec4 FragPos;

in VS_DATA {
	vec3 position;
	vec2 tex;
	mat3 TBN;
} vs_in;

struct Material {
    sampler2D diffuse_1;
    sampler2D normal_1;
    sampler2D specular_1;
};

uniform Material material;

struct lightSource {
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  vec3 spot_direction;

  float constant_attenuation;
  float linear_attenuation;
  float quadratic_attenuation;
  float spot_cutoff;
  float spot_exponent;
  
  bool enabled;
};

layout (std140) uniform Camera {
    vec3 position;
} camera;

const int NR_LIGHTS = 16;

uniform bool mapping_enabled;
uniform float gamma = 2.2f;
uniform lightSource lights[NR_LIGHTS];

vec3 CalcDirectionalLight(lightSource light, vec3 light_dir, vec3 view_dir, vec3 normal)
{
	return vec3(0.0, 0.0, 0.0);
}

vec3 CalcPointLight(lightSource light, vec3 light_dir, vec3 view_dir, vec3 normal)
{
	if(!light.enabled) {
		return vec3(0, 0, 0);
	}

	vec3 ambient = vec3(light.ambient);
	vec3 diffuse = vec3(light.diffuse);
	vec3 specular = vec3(light.specular);

	vec3 halfway_dir = normalize(light_dir + view_dir);
	vec3 reflect_dir = reflect(-light_dir, normal);

	float diff = max(dot(light_dir, normal), 0.0f);
	float spec = pow(max(dot(normal, halfway_dir), 0.0f), 32.0);

	/*
	 * se o ângulo entre a view e a normal
     * for maior que 90º, então não temos luz na outra face
	 */
	if (dot(view_dir, normal) < 0.0f) {
		diff = spec = 0.0f;
	}

	diffuse = diff * diffuse;
	specular = spec * specular;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(lightSource light, vec3 light_dir, vec3 view_dir, vec3 normal)
{
	return vec3(0.0, 0.0, 0.0);
}

vec3 CalcLight(lightSource light, vec3 light_dir, vec3 view_dir, vec3 normal)
{
	if(light.position.w == 1.0f) {
		// directional
		return CalcDirectionalLight(light, light_dir, view_dir, normal);
	} else {
		// point
		return CalcPointLight(light, light_dir, view_dir, normal);
	}
}

vec4 gammaCorrection(vec4 color)
{
    return pow(color, 1.0 / vec4(gamma));
}

void main()
{
	vec3 result = vec3(0.05);

	vec3 normal_mapping = texture(material.normal_1, vs_in.tex).rgb * 2.0 - 0.5;
	normal_mapping = normalize(vs_in.TBN * normal_mapping);

	for(int l = 0; l < NR_LIGHTS; ++l) {
		if(!lights[l].enabled)
			continue;

		vec3 light_dir = normalize(vec3(lights[l].position) - vs_in.position);
		vec3 view_dir = normalize(camera.position - vs_in.position);
	
		if(mapping_enabled) {
			result += CalcLight(lights[l], light_dir, view_dir, normalize(normal_mapping));
		} else {
			result += CalcLight(lights[l], light_dir, view_dir, normalize(vs_in.TBN[2]));
		}
	}

	FragColor = vec4(texture(material.diffuse_1, vs_in.tex).rgb * result, 1.0f);
}