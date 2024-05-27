#version 330 core
out vec4 FragColor;

in VS_DATA {
	vec4 frag_position;
	vec3 normal;
	vec2 tex;
} vs_in;

uniform bool isSpecular;
uniform sampler2D _texture;

void main()
{
	if(isSpecular) {
		float a = texture(_texture, vs_in.tex).a;
		FragColor = vec4(a, a, a, 1.0);
	} else {
		vec3 _rgb = texture(_texture, vs_in.tex).rgb;
		FragColor = vec4(_rgb, 1.0);
	}
}