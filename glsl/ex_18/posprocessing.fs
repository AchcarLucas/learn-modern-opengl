#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

vec3 inverse_color(vec3 color) 
{
    return vec3(1.0f) - color;
}

vec3 inverse_color(vec4 color) 
{
    return vec3(1.0f) - vec3(color);
}

vec3 gray_scale(vec3 color, vec3 weight) 
{
    float average = (color.r * weight.r + color.g * weight.g + color.b * weight.b) / 3.0f;
    return vec3(average);
}

vec3 gray_scale(vec4 color, vec3 weight)
{
    float average = (color.r * weight.r + color.g * weight.g + color.b * weight.b) / 3.0f;
    return vec3(average);
}

// kernel pos-processing
vec3 kernel_color(float _kernel[9], float _offset)
{
    vec2 offset[9] = vec2[] (
        vec2(-_offset,  _offset), // top-left
        vec2( 0.0f,    _offset), // top-center
        vec2( _offset,  _offset), // top-right
        vec2(-_offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( _offset,  0.0f),   // center-right
        vec2(-_offset, -_offset), // bottom-left
        vec2( 0.0f,   -_offset), // bottom-center
        vec2( _offset, -_offset)  // bottom-right    
    );

    vec3 sample_tex[9];

    for(int i = 0; i < 9; ++i) {
        sample_tex[i] = vec3(texture(screenTexture, TexCoords.st + offset[i]));
    }

    vec3 color = vec3(0.0);

    for(int i = 0; i < 9; ++i) {
        color += sample_tex[i] * _kernel[i];
    }

    return color;
}

float offset = 1.0f / 300.0f;

// example
float kernel_1[9] = float[] (
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

// blur
float kernel_2[9] = float[] (
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
);

// edge detector
float kernel_3[9] = float[] (
    1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

float kernel_4[9] = float[] (
    -1,  -1, -1,
    -1, 8, -1,
    -1,  -1, -1
);

// sharpen
float kernel_5[9] = float[] (
    0,  -1, 0,
    -1, 5, -1,
    0,  -1, 0
);

void main()
{ 
    // inverse color
    //FragColor = vec4(inverse_color(texture(screenTexture, TexCoords)), 1.0f);

    // gray color
    //FragColor = vec4(gray_scale(texture(screenTexture, TexCoords), vec3(0.2126f, 0.7152f, 0.0722f)), 1.0f);

    vec3 color = kernel_color(kernel_5, offset);
    FragColor = vec4(color, 1.0f);
}