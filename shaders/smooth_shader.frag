#version 120

varying vec3 vertex_pos;
varying vec3 vertex_nor;
varying vec3 vertex_col;

uniform mat4 MVP;
uniform vec3 COL;
uniform mat4 FILL;
uniform mat4 SIDE;
uniform mat4 BACK;
uniform mat4 M;
uniform mat4 V;
uniform vec3 FOG_COL;
uniform float FOG;

vec3  smoothLight(mat4 light_matrix, vec3 mater_color);

void main(){
  vec3 Position_worldspace = (M * vec4(vertex_pos,1)).xyz;
  vec3 col = smoothLight(FILL, color)
}



