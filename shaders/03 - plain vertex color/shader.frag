#version 410 core

in vec3 fragmentColor;
out vec4 color;

uniform mat4 M;
uniform mat4 V;
uniform mat4 MVP;
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexUV;
layout(location = 3) in vec3 vertexNormal;
vec3 light(vec3 mater_color, int light);

void main(void){
    color = vec4(vertexNormal,1);
    //color = vec4(light(vec3(1,1,1),1),1);
}