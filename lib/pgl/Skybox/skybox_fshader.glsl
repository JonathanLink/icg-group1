#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main() {    
	vec3 white = vec3(1.0f,1.0f,1.0f);
    color =  vec4(mix(texture(skybox, TexCoords).rgb, white, 0.8), 1.0f);

    /*float distance = 60;
    float fogAmount = exp(distance * 0.009) - 1;
    fogAmount = clamp(fogAmount, 0, 0.8);
    vec3  fogColor  = vec3(1,1,1);
    color = vec4(mix( texture(skybox, TexCoords).rgb, fogColor, fogAmount ), 1.0f);*/

}