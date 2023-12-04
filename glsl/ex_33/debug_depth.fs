#version 330 core
out vec4 FragColor;

in VS_DATA {
	vec3 position;
	vec3 normal;
	vec2 tex;
} vs_in;

struct Light {
  vec3 position;

  mat4 view;

  float near_plane;
  float far_plane;
};

uniform Light light;

uniform sampler2D depth_map;

// required when using a perspective projection matrix
float LinearizeDepth(float depth, float near_plane, float far_plane)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    float depth_value = texture(depth_map, vs_in.tex).r;
    // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    FragColor = vec4(vec3(depth_value), 1.0); // orthographic
}