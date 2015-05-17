#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 uv_coords;
in vec3 fragPos;
in float fragHeight;
in vec3 normal;

uniform sampler2D tex;
uniform float grid_size;
uniform sampler2D grassTex;
uniform sampler2D snowTex;
uniform sampler2D sandTex;
uniform sampler2D rockTex;
uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 color;

float grassCoeff(float local_slope) {
    if (local_slope >= 0.7) {
        return 0.0;
    } else if (local_slope <= 0.3) {
        return 0.7;
    } else {
        return local_slope * local_slope * local_slope;
    }
}

float snowCoeff(float local_slope) {
    if (local_slope >= 0.9) {
        return 0.0;
    } else { 
        return 1.0;
    } 
}

float rand(vec2 c){
    float result = fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
    return result;
}

void main() {

    // ============ Texturing part ==================
    float tilingScaleSand = 30;
    float tilingScaleRock = 10;
    float tilingScaleGrass = 15;
    float tilingScaleSnow = 30;

    float angle = dot(normal, vec3(0.0f, -1.0f, 0.0f));
    vec3 textureColor;

    vec3 sandTex = texture(sandTex, tilingScaleSand * uv_coords).rgb;
    vec3 rockTex = texture(rockTex, tilingScaleRock * uv_coords).rgb;
    vec3 grassTex = texture(grassTex, tilingScaleGrass * uv_coords).rgb;
    vec3 snowTex = texture(snowTex, tilingScaleSnow * uv_coords).rgb;
    vec3 rockMixedGrass = mix(rockTex, grassTex, grassCoeff(1.0 - angle));
    vec3 snowMixedRock = mix(rockTex, snowTex, snowCoeff(1 - angle));
    vec3 snowMixedRockMixedGrass = mix(rockMixedGrass, snowTex, snowCoeff(1 - angle)); 

    float pseudoRN = angle / 6;
    float sandHeight = 0.45;
    float sandHeightMixed = sandHeight + 0.05;
    float snowHeight = 0.87;
    float snowHeightMixed = snowHeight + 0.08;

    if (fragHeight + pseudoRN <= sandHeight ) { 
        textureColor = sandTex;
    } else if (fragHeight + pseudoRN <= sandHeightMixed) {
        textureColor = mix(rockMixedGrass, sandTex, (1 - ((fragHeight + pseudoRN) - sandHeight) * 1 / (sandHeightMixed - sandHeight)));
    } else if (fragHeight + pseudoRN <= snowHeight) {
        textureColor = rockMixedGrass;
    } else if (fragHeight + pseudoRN <= snowHeightMixed) {   
        textureColor = mix(snowTex, rockMixedGrass, 1 - ((fragHeight + pseudoRN) - snowHeight) * 1 / (snowHeightMixed - snowHeight));
    } else {
        textureColor = mix(rockMixedGrass, snowTex, snowCoeff(1 - angle));
    }
    

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
    vec3 result = (ambient + diffuse) * textureColor;
    color = vec4(result, 1.0f);


    //===== DEBUG ====

    //color = vec4(mod(angle,2.0*M_PI) /(2.0*M_PI), mod(angle,2.0*M_PI) /(2.0*M_PI), mod(angle,2.0*M_PI) /(2.0*M_PI), 1.0f);
    //vec2 relFragPos = (fragPos.xz + vec2(1.0, 1.0)) * 0.5;
    //color = texture(tex, relFragPos);

    /*
    if (angle < 0.2) {
        color = vec4(1,0,0,0);
    } else if (angle < 0.5) {
        color = vec4(0,1,0,0);
    } else if (angle < 1.0) {
        color = vec4(0,0,1,0);
    }
    */

    
}
