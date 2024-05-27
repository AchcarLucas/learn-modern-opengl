#version 330 core

in GS_DATA {
	vec4 frag_position;
} gs_in;

struct Light {
  vec3 position;

  mat4 views[6];

  float near_plane;
  float far_plane;
};

uniform Light light;

void main()
{
    // get distance between fragment and light source
    float light_distance = length(gs_in.frag_position.xyz - light.position);
    
    // map to [0;1] range by dividing by far_plane
    light_distance = light_distance / light.far_plane;
    
    // write this as modified depth
    gl_FragDepth = light_distance;
}  