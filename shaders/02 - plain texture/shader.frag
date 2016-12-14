#version 410 core

in vec2 uv;
out vec4 color;

uniform sampler2D myTextureSampler;

void main(void){
    color = vec4(texture( myTextureSampler, uv ).rgb,1);
}