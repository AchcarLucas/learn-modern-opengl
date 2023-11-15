#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_DATA {
    vec3 position;
    vec3 normal;
    vec2 tex;
} vs_in[];

out GS_DATA {
    vec2 tex;
    vec3 normal;
} gs_out;

layout (std140) uniform Global {
    float time;
} global;

vec3 getNormal() {
    vec3 first_vector = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 second_vector = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    return normalize(cross(first_vector, second_vector));
}

vec4 explode_dir(vec4 position, vec3 normal) {
    float f_sin = ((sin(global.time) + 1.0) / 2.0);
    float magnitude = 2.0f;
    vec3 direction = normal * f_sin * magnitude;
    return vec4(direction, 1.0f);
}

void main() {
    vec3 _normal = getNormal();

    gl_Position = gl_in[0].gl_Position + explode_dir(gl_in[0].gl_Position, _normal);
    gs_out.tex = vs_in[0].tex;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + explode_dir(gl_in[1].gl_Position, _normal);
    gs_out.tex = vs_in[1].tex;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position + explode_dir(gl_in[2].gl_Position, _normal);
    gs_out.tex = vs_in[2].tex;
    EmitVertex();

    EndPrimitive();
}  