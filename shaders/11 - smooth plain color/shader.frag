#version 410 core

uniform vec3 col;
uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;

in vec3  pos;
in vec3  normal;
out vec4 color;

vec3 smoothLight(vec3 color);

void main(void){
    color = vec4(smoothLight(col),1);
}

vec3 smoothLight(vec3 color){
    vec3  light_posit     = vec3(2,2,2);
    vec3  light_color     = vec3(1,1,1);
    vec3  mix_ratio       = vec3(1,1,1);
    float light_power     = 10.0f;
    float lobe_size       = 5.0f;
    bool  camera_anchored = bool(1);
    
    float distance = length(light_posit - (MVP * vec4(pos,1)).xyz);
    vec3 vertex_position_cameraspace = ( V * M * vec4(pos,1)).xyz;
    vec3 eye_direction_cameraspace = vec3(0,0,0) - vertex_position_cameraspace;
    vec3 light_position_cameraspace = ( V * vec4(light_posit,1)).xyz;
    
    //WORLD OR CAMERA?
    vec3 light_direction_cameraspace;
    if(camera_anchored)
        light_direction_cameraspace = light_posit + eye_direction_cameraspace;
    else
        light_direction_cameraspace = light_position_cameraspace + eye_direction_cameraspace;
    
    vec3 normal_cameraspace = ( V * M * vec4(normal,0)).xyz;
    vec3 n = normalize( normal_cameraspace );
    vec3 l = normalize( light_direction_cameraspace );
    float cosTheta = clamp( dot( n,l ), 0.,1. );
    
    vec3 E = normalize(eye_direction_cameraspace);
    vec3 R = reflect(-l,n);
    float cosAlpha = clamp( dot( E,R ), 0.,1. );
    
    vec3 ambi_color = 0.1 *  color;
    vec3 diff_color = color;
    vec3 spec_color = 0.5 * (color + vec3(1.0, 1.0, 1.0));
    vec3 col = mix_ratio.x * ambi_color
    + mix_ratio.y * diff_color * light_color * light_power * cosTheta        / (distance*distance)
    + mix_ratio.z * spec_color * light_color * light_power * pow(cosAlpha,lobe_size) / (distance*distance)
    ;
    
    return col;
}