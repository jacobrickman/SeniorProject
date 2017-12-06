#version 330 core

layout(location = 0) out vec4 outColor;

uniform vec3 inColor;

in vec3 L;
in vec3 normal;

void main()
{
	float intensity;
	vec3 color;
	intensity = dot(L,normalize(normal));

	if (intensity > 0.95)
	{
		//color = vec3(1.0,0.5,0.5);
		color = vec3(1.0, 1.0, 1.0);
	}
	else if (intensity > 0.5)
	{
		//color = vec3(0.6,0.3,0.3);
		color = vec3(.7, .7, .7);
	}
	else if (intensity > 0.25)
	{
		//color = vec3(0.4,0.2,0.2);
		color = vec3(.35, .35, .35);
	}
	else
	{
		//color = vec3(0.2,0.1,0.1);
		color = vec3(.1, .1, .1);
	}

	outColor = vec4(color * inColor, 1.0);
}