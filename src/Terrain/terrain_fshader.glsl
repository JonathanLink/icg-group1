#version 330 core

in float my_height;
in float diffuse_coefficient;

uniform sampler2D grassTex;
uniform sampler2D snowTex;
uniform sampler2D sandTex;
uniform sampler2D rockTex;  

out vec3 color;
in vec2 uv;


void main() {

    float tilingScale = 10;

    if (my_height <= 0.39) {
    	 color = texture(sandTex, tilingScale * uv).rgb * diffuse_coefficient;
    } else if (my_height <= 0.47) {
        color = texture(grassTex, tilingScale *uv).rgb * diffuse_coefficient;
    } else if (my_height <= 0.85) {
        color = mix(texture(rockTex, tilingScale * uv).rgb, texture(grassTex, tilingScale * uv).rgb, 0.6) * diffuse_coefficient;
    } else {
        color = texture(snowTex, tilingScale * uv).rgb * diffuse_coefficient;
    }


    //color = vec3(height0, height0, height0);
    //color *= diffuseLength * 5.0;
    //color = vec3(1.0, 0.0, 0.0);
}
