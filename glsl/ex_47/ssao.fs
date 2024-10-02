#version 330 core

out vec4 FragColor;

in VS_DATA {
	vec2 tex;
} vs_in;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D tNoise;

uniform vec3 samples[64];

uniform int kernelSize;
uniform float radius;
uniform float bias;

uniform float width;
uniform float height;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
} matrices;

void main()
{
    vec2 noiseScale = vec2(width / 4.0, height / 4.0);

    // get input for SSAO algorithm
    vec3 fragPos = texture(gPosition, vs_in.tex).xyz;
    vec3 normal = normalize(texture(gNormal, vs_in.tex).rgb);
    vec3 randomVec = normalize(texture(tNoise, vs_in.tex * noiseScale).xyz);
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 samplePos = TBN * samples[i]; // from tangent to view-space
        samplePos = fragPos + samplePos * radius;
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(samplePos, 1.0);
        offset = matrices.projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = vec4(occlusion);

}