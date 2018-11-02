#version 330

in vec4 pos;

out vec3 frag_coord;
out vec2 tex_coord;

void main()
{	
	frag_coord = pos.xyz;
	tex_coord = (pos.xy + vec2(1.0)) / 2.0;
    gl_Position = pos;
}