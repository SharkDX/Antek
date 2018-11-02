#version 330
uniform mat4 MVP;
uniform float ambient = 0.5;
uniform vec3 lightDir = vec3(1, -3, 2);

in vec3 pos;
in vec3 normal;
in vec3 color;
in float blocklight;

out vec3 vec_color;

void main()
{	
    gl_Position = MVP * vec4(pos, 1);
	float light = clamp(ambient + dot(normalize(normal), normalize(-lightDir)) * (1-ambient), 0, 1);
	vec_color = color * light * blocklight;
}