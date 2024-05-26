#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_DATA {
	vec4 frag_model_position;
	vec3 normal;
	vec2 tex;
} vs_in;

struct Material {
    sampler2D diffuse_1;
    sampler2D normal_1;
    sampler2D specular_1;
};

uniform Material material;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = vs_in.frag_model_position.xyz;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(vs_in.normal);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.diffuse_1, vs_in.tex).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(material.specular_1, vs_in.tex).r;
} 