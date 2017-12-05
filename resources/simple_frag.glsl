#version 330 core 
in vec3 fragNor;
in vec3 WPos;
//to send the color to a frame buffer
layout(location = 0) out vec4 color;

in vec3 N;
in vec3 L;
in vec3 H;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform vec3 LightColor;
uniform float S;

/* Very simple Diffuse shader with a directional light*/
void main()
{
	//vec3 Dcolor, Scolor;
    //vec3 Dlight = vec3(1, 1, 1);
	//vec3 normal = normalize(fragNor);
	//Dcolor = MatDif*max(dot(normalize(Dlight), normal), 0)+MatAmb;
	//color = vec4(Dcolor, 1.0);

	vec3 nN = normalize(N);
	vec3 nL = normalize(L);
	vec3 nH = normalize(H);

	color = vec4((MatDif * (max(dot(nN, nL), 0)) * LightColor + MatSpec * pow((max(dot(nN, nH), 0)),S) * LightColor + MatAmb), 0);
}
