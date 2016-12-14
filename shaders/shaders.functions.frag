#version 410 core

in vec3 fragmentColor;
in vec2 uv;
out vec4 color;

uniform sampler2D myTextureSampler;

uniform vec3 col;

void main(void){
    //color = vec4(0,0,1,1);
		//color = vec4(fragmentColor,1);
		color = vec4(texture( myTextureSampler, uv ).rgb,1);
}