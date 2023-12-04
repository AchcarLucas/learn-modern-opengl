#version 330 core
out vec4 FragColor;

in VS_DATA {
	vec4 frag_position;
	vec3 normal;
	vec2 tex;
} vs_in;

uniform samplerCube depth_cubemap;

void main()
{             
    FragColor = vec4(texture(depth_cubemap, vec3(vs_in.tex - 0.5, 1.0f)).rgb, 1.0);
}