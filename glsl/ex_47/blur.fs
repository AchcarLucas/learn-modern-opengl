#version 330 core
out vec4 FragColor;

in VS_DATA {
    vec2 tex;
} vs_in;

uniform sampler2D _texture;

uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(_texture, 0); // gets size of single texel
    vec3 result = texture(_texture, vs_in.tex).rgb * weight[0];

    if(horizontal) {
        for(int i = 1; i < 5; ++i) {
            result += texture(_texture, vs_in.tex + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(_texture, vs_in.tex - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    } else {
        for(int i = 1; i < 5; ++i) {
            result += texture(_texture, vs_in.tex + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(_texture, vs_in.tex - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    FragColor = vec4(result, 1.0);
} 