#version 330 core
in vec2 TexCoords;
out vec4 frag_Color;

//uniform sampler2D screenTexture;
uniform sampler2D G_PositionDepth;

uniform float cameraFocal;
uniform float cameraAperture;
uniform float planeInFocus;

uniform mat4 invPersp;

void main()
{ 


	float sampleDepth = texture(G_PositionDepth, TexCoords).w; 
	float focus = planeInFocus;
	
	float f = cameraFocal;
	float aperture = cameraAperture;

	float CoC = (aperture * (f * abs(sampleDepth - focus))/(sampleDepth * abs(planeInFocus - f)));
	frag_Color.rgb = vec3(CoC);
}