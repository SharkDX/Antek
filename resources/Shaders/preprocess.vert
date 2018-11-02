#version 330
uniform mat4 MVP;
uniform mat4 MV;
uniform float ambient = 0.5;
uniform vec3 lightDir = vec3(1, -3, 2);

in vec3 pos;
in vec3 normal;
in vec3 color;
in float blocklight;

out vec3 vec_pos;
out vec3 vec_normal;

void main()
{	
    gl_Position = MVP * vec4(pos, 1);
    vec_normal = normalize(normal);
	float light = clamp(ambient + dot(vec_normal, normalize(-lightDir)) * (1-ambient), 0, 1);
	//vec_color = color * light * blocklight;
	vec_pos = (pos).xyz;
}