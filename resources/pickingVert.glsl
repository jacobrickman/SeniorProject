#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertPos;

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 MV;

void main(){

    // Output position of the vertex, in clip space : MVP * position
    gl_Position = P * MV * vec4(vertPos,1);

}