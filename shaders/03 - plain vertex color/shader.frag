#version 410 core

in vec3 fragmentColor;
out vec4 color;

void main(void){
    color = vec4(fragmentColor,1);
}