#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emission;

    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;

    bool enabled;
};

struct SpotLight {
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

    bool enabled;
};

#define NR_POINT_LIGHTS 4  
#define NR_SPOT_LIGHTS 4  

uniform float iTime;

uniform vec3 viewPos;

uniform Material material;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform SpotLight spot_lights[NR_SPOT_LIGHTS];

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    if(!light.enabled)
        return vec3(0.0f, 0.0f, 0.0f);

    vec3 light_dir = normalize(light.position - frag_pos);
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 frag_pos)
{
    if(!light.enabled)
        return vec3(0.0f, 0.0f, 0.0f);

    vec3 light_dir = normalize(light.position - frag_pos);

    float diff = max(dot(normal, light_dir), 0.0);

    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.inner_cutoff - light.outer_cutoff;

    float intensity = smoothstep(0.0, 1.0, (theta - light.outer_cutoff) / epsilon) * diff;

    // combine results
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse  = light.diffuse *  vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.specular *  vec3(texture(material.specular, texCoord));
    ambient  *= intensity;
    diffuse  *= intensity;
    specular *= intensity;
    return (ambient + diffuse + specular);
}

void main()
{
	// properties
    vec3 norm = normalize(fragNormal);
    vec3 view_dir = normalize(viewPos - fragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirectionalLight(directional_light, norm, view_dir);

    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(point_lights[i], norm, fragPos, view_dir);    

    // phase 3: Spot light
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += CalcSpotLight(spot_lights[i], norm, fragPos);
    
    FragColor = vec4(result, 1.0);
}