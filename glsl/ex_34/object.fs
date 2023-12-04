#version 330 core

out vec4 FragColor;

in VS_DATA {
	vec4 frag_model_position;
	vec4 frag_shadow_position;
	vec3 normal;
	vec2 tex;
} vs_in;

struct Material {
	sampler2D diffuse_1;
	sampler2D depth_1;
	samplerCube depth_cubemap_1;
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

  float near_plane;
  float far_plane;
  
  bool enabled;
};

layout (std140) uniform Camera {
    vec3 position;
} camera;

const int NR_LIGHTS = 16;

uniform float gamma;
uniform lightSource lights[NR_LIGHTS];

float ShadowCalculationDir(vec4 frag_shadow_position, vec3 light_dir, vec3 normal);
float ShadowCalculationDirPCF(vec4 frag_shadow_position, vec3 light_dir, vec3 normal);

float ShadowCalculationPoint(lightSource light, vec4 frag_model_position, vec3 light_pos);

vec3 CalcPointLight(lightSource light, vec3 light_dir, vec3 view_dir, vec3 normal, float shadow)
{
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
 
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcDirectionalLight(lightSource light, vec3 light_dir, vec3 view_dir, vec3 normal, float shadow)
{
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
 
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcSpotLight(lightSource light, vec3 light_dir, vec3 view_dir, vec3 normal, float shadow)
{
	return vec3(0.0, 0.0, 0.0);
}

vec3 CalcLight(lightSource light, vec3 light_dir, vec3 view_dir, vec3 normal)
{
	if(light.position.w == 1.0f) {
		// point
		float shadow = ShadowCalculationPoint(light, vs_in.frag_model_position, light.position.xyz);
		return CalcPointLight(light, light_dir, view_dir, normal, shadow);
	} else {
		// directional
		// float shadow = ShadowCalculationDir(vs_in.frag_shadow_position, light_dir, normal);
		float shadow = ShadowCalculationDirPCF(vs_in.frag_shadow_position, light_dir, normal);
		return CalcDirectionalLight(light, light_dir, view_dir, normal, shadow);
	}
}

float CalcAttenuation(vec3 light_position, vec3 frag_position)
{
	vec3 _length = frag_position - light_position;
	return pow(1.0 / abs(length(_length)), gamma);
}

float ShadowCalculationDir(vec4 frag_shadow_position, vec3 light_dir, vec3 normal)
{
	// perform perspective divide and transform to [0,1] range
	vec3 norm_frag_shadow = (frag_shadow_position.xyz / frag_shadow_position.w) * 0.5f + 0.5f;

	if(norm_frag_shadow.z > 1.0) 
		return 0.0f;

	float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);  

	// get closest depth value from light's perspective
	float closest_depth = texture(material.depth_1, norm_frag_shadow.xy).r;
	// get depth of current fragment shader from light's perspective
	float current_depth = norm_frag_shadow.z;

	return (current_depth - bias > closest_depth ? 1.0f : 0.0f);
}

float ShadowCalculationDirPCF(vec4 frag_shadow_position, vec3 light_dir, vec3 normal)
{
	// perform perspective divide and transform to [0,1] range
	vec3 norm_frag_shadow = (frag_shadow_position.xyz / frag_shadow_position.w) * 0.5f + 0.5f;

	if(norm_frag_shadow.z > 1.0) 
		return 0.0f;

	// get depth of current fragment shader from light's perspective
	float current_depth = norm_frag_shadow.z;

	float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);  

	float shadow = 0.0f;
	vec2 texel_size = 1.0 / textureSize(material.depth_1, 0);

	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcf_depth = texture(material.depth_1, norm_frag_shadow.xy + vec2(x, y) * texel_size).r; 
			shadow += (current_depth - bias) > pcf_depth ? 1.0 : 0.0;        
		}    
	}

	return (shadow / 9.0f);
}

float ShadowCalculationPoint(lightSource light, vec4 frag_model_position, vec3 light_pos)
{
    // get vector between fragment position and light position
    vec3 frag_to_light = frag_model_position.xyz - light_pos;
    // ise the fragment to light vector to sample from the depth map    
    float closest_depth = texture(material.depth_cubemap_1, frag_to_light).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closest_depth *= light.far_plane;
    // now get current linear depth as the length between the fragment and light position
    float current_depth = length(frag_to_light);
    // test for shadows
    float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = current_depth -  bias > closest_depth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
	vec3 result = vec3(0.05f);

	for(int l = 0; l < NR_LIGHTS; ++l) {
		if(!lights[l].enabled)
			continue;

		vec3 light_dir = normalize(vec3(lights[l].position) - vec3(vs_in.frag_model_position));
		vec3 view_dir = normalize(camera.position - vec3(vs_in.frag_model_position));
	
		result += CalcLight(lights[l], light_dir, view_dir, normalize(vs_in.normal));
		result *= CalcAttenuation(vec3(lights[l].position), vec3(vs_in.frag_model_position));
	}

	FragColor = vec4(texture(material.diffuse_1, vs_in.tex).rgb * result, 1.0f);
}