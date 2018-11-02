#version 330

in vec3 vec_pos;
in vec3 vec_normal;

out vec3 fragColor;
//out vec4 fragNormal;

void main()
{
    fragColor = vec_pos;
	//fragNormal = vec4(normalize(vec_normal) * 0.5 + 0.5, 1.0);
}