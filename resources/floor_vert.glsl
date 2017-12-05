#version  330 core
layout(location = 0) in vec4 vertPos;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec3 WPos;

void main()
{
	gl_Position = P * V *  M * vertPos;

	WPos = vec3(V * M * vertPos);
}