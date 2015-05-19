#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 uv_coords;
in vec3 fragPos;
in vec3 normal;
in float fragHeight;

uniform sampler2D tex;
uniform sampler2D tex_mirror;
uniform float grid_size;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform float water_height;
uniform int fogEnabled;

out vec4 color;


void main() {

    vec4 textureColor = vec4(0,128.0/255.0,1.0, 0.6);
    vec4 finalColor = vec4(1,1,1,1);
   
    if (fragHeight > water_height) {
        finalColor = vec4(0.0,0.0,0.0,0.0); 
    } else {

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
        finalColor = result;


        // ============ Reflection part ==================

        vec2 whSize = textureSize(tex_mirror, 0);
        float window_width = whSize.x;
        float window_height = whSize.y;

        float u = gl_FragCoord.x / window_width;
        float v = gl_FragCoord.y / window_height;


        vec3 terrainReflected = texture(tex_mirror, vec2(u,v)).rgb;

        finalColor.xyz = mix(finalColor.xyz, terrainReflected, 0.45);


        // ============ Fog part =======================

        if (fogEnabled > 0.5) { // not == 1 to avoid float procession error
            float distance = distance(cameraPos, fragPos);
            float fogAmount = exp(distance * 0.009) - 1;
            fogAmount = clamp(fogAmount, 0, 0.8);
            vec4  fogColor  = vec4(1,1,1,1);
            finalColor = mix( result, fogColor, fogAmount );
        }
        
    }

    color = finalColor;
}
