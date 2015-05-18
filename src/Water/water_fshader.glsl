#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 uv_coords;
in vec3 fragPos;

uniform sampler2D tex;
uniform float grid_size;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

out vec4 color;


void main() {

    color = vec4(0,0,1.0,0.3);
    
}
