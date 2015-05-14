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
uniform vec3 cameraPos;

out vec4 color;

float grassCoeff(float local_slope) {
    if (abs(local_slope) >= 0.7) {
        return 0.0;
    } else if (abs(local_slope) <= 0.3) {
        return 1.0;
    } else {
        return local_slope * local_slope * local_slope;
    }
}


void main() {

    // ============ Texturing part ==================
    float tilingScalaSand = 30;
    float tilingScaleGrassRock = 10;
    float tilingScaleSnow = 60;

    float angle = dot(normal, vec3(0.0f, -1.0f, 0.0f));
    vec3 textureColor;
    

    if (fragHeight <= 0.39) { // sand
         textureColor = texture(sandTex, 6 * tilingScalaSand * uv_coords).rgb;
    } else if (fragHeight <= 0.8) { // rock
        textureColor = mix(texture(rockTex, tilingScaleGrassRock * uv_coords).rgb, texture(grassTex, tilingScaleGrassRock * uv_coords).rgb, grassCoeff(1.0 - angle));
    } else { // snow
        textureColor = texture(snowTex, 3 * tilingScaleSnow * uv_coords).rgb ;
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
    //color = vec4(result, 1.0f);


    // ============ Fog part =======================
    /*float distance = distance(cameraPos, fragPos);
    vec3 rayDir =  fragPos - cameraPos;
    float b = 0.01;
    float fogAmount = 1.0 - exp( -distance * b );
    float sunAmount = max( dot( rayDir, lightPos ), 0.0 );
    vec3  fogColor  = mix( vec3(0.5,0.6,0.7), // bluish
                           vec3(1.0,0.9,0.7), // yellowish
                           pow(sunAmount,8.0) );
    color = vec4(mix( result, fogColor, fogAmount ), 1.0f);*/
    ////

    /*float b  = 0.05;
    float c = 0.5;
    float distance = distance(cameraPos, fragPos);
    vec3 rayDir =  fragPos - cameraPos;
    float fogAmount = c * exp(-cameraPos.y*b) * (1.0-exp( -distance*rayDir.y*b ))/rayDir.y;
    //vec3  fogColor  = vec3(1,1,1);
     float sunAmount = max( dot( rayDir, lightPos ), 0.0 );
    vec3  fogColor  = mix( vec3(0.5,0.6,0.7), // bluish
                           vec3(1.0,1.0,1.0), // yellowish
                           pow(sunAmount,2.0) );
    color = vec4(mix( result, fogColor, fogAmount ), 1.0f);
    */
    ////

    float b  = 0.009;
    float c = 0.5;
    float distance = distance(cameraPos, fragPos);
    vec3 rayDir =  fragPos - cameraPos;
    float fogAmount = c * exp(-cameraPos.y*b) * (1.0-exp( -distance*rayDir.y*b ))/rayDir.y;
    vec3  fogColor  = vec3(1,1,1);
    color = vec4(mix( result, fogColor, fogAmount ), 1.0f);
    


    /*
    float b  = 0.01;
    const float LOG2 = 1.442695;
    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = exp2( -b *  b * z * z * LOG2 );
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3  fogColor  = vec3(1,1,1);
    color = vec4(mix(fogColor, result, fogFactor ), 1.0f);
    */

    
}
