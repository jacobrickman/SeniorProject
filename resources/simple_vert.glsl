#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;


uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform vec3 Lpos;

out vec3 N;
out vec3 L;
out vec3 H;

void main()
{
	gl_Position = P * V *  M * vertPos;

	//vec4 vertNor2 = V * M * vec4(vertNor, 0);
	vec4 vertNor2 = vec4(vertNor, 0);

	vec3 wPos = vec3(V * M * vertPos);
	vec3 view = -1 * wPos;
	vec3 V = normalize(view);

	L = Lpos - wPos;
    L = normalize(L);

    H = L + V;
    H = normalize(H);

    N = normalize(vec3(vertNor2));
}
