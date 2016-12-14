#version 410 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;

out vec3 normal;
out vec3 pos;

void main(void){
    gl_Position = MVP * vec4(vertexPosition,1);
    normal = vertexNormal;
    pos = vertexPosition;
}