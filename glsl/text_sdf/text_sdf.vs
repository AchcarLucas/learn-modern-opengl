#version 330 core

// <vec2 pos, vec2 tex>
layout (location = 0) in vec4 vertex;

out VS_DATA {
	vec2 tex;
} vs_out;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vertex.xy, 0.0, 1.0);
    vs_out.tex = vertex.zw;
}  