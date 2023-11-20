#version 330 core

out vec4 FragColor;

void main() {
    //gl_FragDepth = gl_FragCoord.z;
    FragColor = vec4(gl_FragCoord.z, 0.0f, 0.0f, 1.0f);
}