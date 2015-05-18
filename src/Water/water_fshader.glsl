#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 uv_coords;
in vec3 fragPos;
in float fragHeight;
in vec3 normal;

uniform sampler2D tex;
uniform float grid_size;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

out vec4 color;


void main() {

    vec4 textureColor = vec4(0,0,1.0,0.3);
    

     // ============ Lightning part ==================

    // Ambient
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Ambient + Diffuse
    vec4 result = vec4((ambient + diffuse), 1.0f) * textureColor;
    //color = vec4(result, 1.0f);


    // ============ Fog part =======================
    float distance = distance(cameraPos, fragPos);
    float fogAmount = exp(distance * 0.009) - 1;
    fogAmount = clamp(fogAmount, 0, 0.8);
    vec4  fogColor  = vec4(1,1,1,1);
    color = mix( result, fogColor, fogAmount );
    
    //color = textureColor;

}
