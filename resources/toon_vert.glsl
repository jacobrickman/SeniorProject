#version 330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform vec3 Lpos;

out vec3 L;
out vec3 normal;

//varying vec3 normal;

void main()
{
	normal = normalize(vertNor);
	gl_Position = P * V * M * vertPos;

	vec3 wPos = vec3(V * M * vertPos);

	L = Lpos - wPos;
	L = normalize(L);
}