#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
} matrices;

out VS_DATA {
    vec2 tex_coords;
    vec3 normal;
    vec3 position;
} vs_out;

uniform mat4 model;

void main()
{
    vs_out.tex_coords = aTexCoords;
    // https://www.songho.ca/opengl/gl_normaltransform.html
    vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.position = vec3(model * vec4(aPos, 1.0));
    gl_Position = matrices.projection * matrices.view * model * vec4(aPos, 1.0);
}
