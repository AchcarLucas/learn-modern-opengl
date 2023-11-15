#version 330 core

out vec4 FragColor;

in GS_DATA {
    vec3 color;
} gs_in;

void main()
{
    FragColor = vec4(gs_in.color, 1.0f);
}