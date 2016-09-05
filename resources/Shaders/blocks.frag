#version 330

in vec3 vec_color;

out vec4 fragColor;

void main()
{
	vec4 color = vec4(vec_color, 1.0);
	
    fragColor = color;
	
}