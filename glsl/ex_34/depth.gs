#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

out GS_DATA {
	vec4 frag_position;
} gs_out;

struct Light {
  vec3 position;

  mat4 views[6];

  float near_plane;
  float far_plane;
};

uniform Light light;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle vertex
        {
            gs_out.frag_position = gl_in[i].gl_Position;
            gl_Position = light.views[face] * gs_out.frag_position;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  