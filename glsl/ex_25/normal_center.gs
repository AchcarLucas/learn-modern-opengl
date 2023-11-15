#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

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

const float MAGNITUDE = 0.02;

void generateCenterLine() {
    vec4 m_pos = vec4(
        (gl_in[0].gl_Position.x + gl_in[1].gl_Position.x + gl_in[2].gl_Position.x) / 3.0f, 
        (gl_in[0].gl_Position.y + gl_in[1].gl_Position.y + gl_in[2].gl_Position.y) / 3.0f,
        (gl_in[0].gl_Position.z + gl_in[1].gl_Position.z + gl_in[2].gl_Position.z) / 3.0f,
        1.0f
    );

    gl_Position = matrices.projection * m_pos;
    EmitVertex();

    gl_Position = matrices.projection * (m_pos + vec4(vs_in[0].normal, 0.0) * MAGNITUDE);
    EmitVertex();

    gs_out.normal = vs_in[0].normal;

    EndPrimitive();
}

void main() {
    generateCenterLine();
}  