#version 330 core

out vec4 color;


in vec2 TexCoords;


uniform sampler2D screenTexture;

const int blurSize = 5;
const float PI = 3.141592653589793238462643383;


float f_domaineWeighting(vec2 p, vec2 s)
{
	float sigma_d = 3.0;
	float d = length(p-s);

	return exp(-(d*d)/(2.0*sigma_d*sigma_d));

}

float g_rangeWeighting(vec3 Ip, vec3 Is)
{
	float sigma_g = 3.0;
	float d = length(Ip-Is);


	return exp(-(d*d)/(2.0*sigma_g*sigma_g));

}

float g_rangeWeighting(vec4 Ip, vec4 Is)
{
	float sigma_g = 3.0;
	float d = length(Ip-Is);
	vec4 Idiff = abs(Ip - Is);
	d = ( (Idiff.r+Idiff.g+Idiff.b) + Idiff.w);



	return exp(-(d*d)/(2.0*sigma_g*sigma_g));

}




void main()
{ 
	
	vec2 texelSize = 1.0 / vec2(textureSize(screenTexture, 0));


	//color = texture(screenTexture, TexCoords);
	/*
	vec2 Value = vec2(0.0);
	float Coefficients[21] = 
    float[](0.000272337, 0.00089296, 0.002583865, 0.00659813, 0.014869116,
     0.029570767, 0.051898313, 0.080381679, 0.109868729, 0.132526984,
     0.14107424,
     0.132526984, 0.109868729, 0.080381679, 0.051898313, 0.029570767,
     0.014869116, 0.00659813, 0.002583865, 0.00089296, 0.000272337);
    for(int Index = 0; Index < 21; Index++)
    {
		vec2 offset = (Index - 10) * texelSize;
        Value += texture(screenTexture, vec2(TexCoords.x, TexCoords.y) + offset).xy * Coefficients[Index];
    }


    color = vec4(Value.x, Value.y, 0.0, 0.0);
	*/


	
	// bi-lateral filter
	// window [-blurSize x blurSize] x [-blurSize x blurSize]
	
	vec4 Is = texture(screenTexture, TexCoords);
	vec3 Js = vec3(0.0,0.0,0.0);
	float Ks = 0.0;

	for (int x = -blurSize; x <= blurSize; ++x) 
    {
        for (int y = -blurSize; y <= blurSize; ++y) 
        {
			vec2 p =  vec2(float(x), float(y));
            vec2 offset = vec2(float(x), float(y)) * texelSize;
			vec4 Ip = texture(screenTexture, TexCoords + offset);

			// color filter
			float weight = f_domaineWeighting(p,vec2(0.0,0.0)) * g_rangeWeighting(Ip.rgb,Is.rgb);

			Js += weight * Ip.rgb;
			Ks += weight;
        }
    }
	Js /= Ks;
	color.rgb = Js;



}