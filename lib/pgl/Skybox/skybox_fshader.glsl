#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;
uniform int fogEnabled;

void main() {    
    vec3 white = vec3(1.0f,1.0f,1.0f);
     float alpha = 0.0;
     if (fogEnabled > 0.5) { // not == 1 to avoid float procession error
        alpha = 0.8;
     } 
    color =  vec4(mix(texture(skybox, TexCoords).rgb, white, alpha), 1.0f);

}