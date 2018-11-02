#version 330

uniform sampler2D NormalBuffer;
uniform sampler2D PosBuffer;
uniform sampler2D DepthBuffer;
uniform sampler2D NoiseBuffer;
uniform vec3 Samples[64];
uniform mat4 Projection;

const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0); // screen = 1280x720

in vec3 frag_coord;
in vec2 tex_coord;

out vec4 fragColor;

void main()
{
	vec3 fragPos   = texture(PosBuffer, tex_coord).xyz;

	int kernelSize = 64;
	float radius = 0.5;
	float bias = 0.0;

	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; i++)
	{
	    // get sample position
	    vec3 sample = fragPos + Samples[i]; 
	    
	    vec4 offset = vec4(sample, 1.0);
		offset      = Projection * offset;    // from view to clip-space
		offset.xy /= offset.w;               // perspective divide
		offset.xy  = offset.xy * 0.5 + vec2(0.5); // transform to range 0.0 - 1.0  

		float sampleDepth = texture(DepthBuffer, offset.xy).r; 

		if (sampleDepth - fragPos.z < 1.5) {
            occlusion += 1.0;
        }
	} 

	occlusion = 1.0 - occlusion / kernelSize;

    fragColor = vec4(pow(occlusion, 2.0));
}