#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
} matrices;

layout (std140) uniform Global {
    float time;
} global;

in VS_DATA {
    vec3 position;
    vec3 normal;
    vec2 tex;
} vs_in[];

out GS_DATA {
    vec2 tex;
    vec3 normal;
} gs_out;

const float MAGNITUDE = 0.4;

void generateLine(int index) {
    gl_Position = matrices.projection * gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = matrices.projection * (gl_in[index].gl_Position + vec4(vs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();

    gs_out.tex = vs_in[index].tex;
    gs_out.normal = vs_in[index].normal;

    EndPrimitive();
}

void main() {
    generateLine(0);
    generateLine(1);
    generateLine(2);
}  