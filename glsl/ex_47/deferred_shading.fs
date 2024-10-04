#version 330 core

layout (location = 0) out vec4 FragColor;

in VS_DATA {
	vec2 tex;
} vs_in;

layout (std140) uniform Camera {
    vec3 position;
} camera;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D Bloom;
uniform sampler2D SSAO;

struct lightSource {
    vec3 position;
    vec3 color;

    float linear;
    float quadratic;

    bool enabled;
};

const int NR_LIGHTS = 32;

uniform float gamma;
uniform lightSource lights[NR_LIGHTS];

uniform bool ssao_enabled;

void main()
{
    vec3 frag_pos = texture(gPosition, vs_in.tex).rgb;
    vec3 frag_normal = texture(gNormal, vs_in.tex).rgb;
    vec3 frag_diffuse = texture(gAlbedoSpec, vs_in.tex).rgb;
    float frag_specular = texture(gAlbedoSpec, vs_in.tex).a;
    float ambient_occlusion = texture(SSAO, vs_in.tex).r;

    // then calculate lighting as usual

    vec3 lighting = (ssao_enabled ? ambient_occlusion : 1.0) * frag_diffuse * 0.8; // hard-coded ambient component
    vec3 view_dir = normalize(camera.position - vec3(frag_pos));

    bool has_light = false;

	for(int l = 0; l < NR_LIGHTS; ++l) {
		if(!lights[l].enabled)
			continue;
        
        has_light = true;

		vec3 light_dir = normalize(vec3(lights[l].position) - vec3(frag_pos));
		vec3 diffuse = max(dot(frag_normal, light_dir), 0.0) * frag_diffuse * lights[l].color;
	
		vec3 halfway_dir = normalize(light_dir + view_dir);  
        float spec = pow(max(dot(frag_normal, halfway_dir), 0.0), 8.0);
        vec3 specular = lights[l].color * spec * frag_specular;

        // attenuation
        float _distance = length(lights[l].position - frag_pos);
        float attenuation = 1.0 / (1.0 + lights[l].linear * _distance + lights[l].quadratic * _distance * _distance);

        diffuse *= attenuation;
        specular *= attenuation;

        lighting += diffuse + specular;
	}

    vec3 bloom = texture(Bloom, vs_in.tex).rgb;

    if(!has_light)
    {
        lighting = frag_diffuse;
    }

	FragColor = vec4(lighting + bloom, 1.0f);
}