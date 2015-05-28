#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 uv_coords;
in vec3 fragPos;
in vec3 normal;
in float fragHeight;
in vec2 frag_coord;
in vec2 posV;

uniform sampler2D tex;
uniform sampler2D tex_mirror;
uniform sampler2D tex_water;
uniform sampler2D tex_water_normal;

uniform float grid_size;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform mat4 MVP_matrix;
uniform float water_height;
uniform int fogEnabled;
uniform float time;

out vec4 color;


void main() {
    //vec4 textureColor = vec4(0,128.0/255.0, 1.0, 0.5);
 
    /* 
    PROBLEM
        Find a way better move equation  (cf. slide technique)
    PROBLEM
    */
    vec2 uv = posV.xy - 0.02*time;

    vec4 textureColor = vec4(texture(tex_water, 5*uv).rgb, 0.7);
    vec4 finalColor = vec4(0,0,0,0);
   
    if (fragHeight >= water_height) { // hide water if not a visible lake
       finalColor = vec4(0.0,0.0,0.0,0.0); 
    } else {

        // ============ Lightning part ==================

        // Ambient
        float ambientStrength = 0.5f;
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse
        vec3 waterNormal = texture(tex_water_normal, uv).rgb;
        vec3 norm = normalize(waterNormal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        //Specular
        vec3 vReflection = normalize(reflect(-normalize(lightDir),normalize(waterNormal)));
        float spec = max(0.0, dot(normalize(waterNormal), vReflection));

        vec3 vSpec = vec3(0.0);

        if(diff != 0) {
            float fSpec = pow(spec, 32.0);
            vSpec = vec3(fSpec, fSpec, fSpec);
        } 


        // Ambient + Diffuse + Specular

        vec4 result = vec4((ambient + diffuse + vSpec), 1.0f) * textureColor;
        finalColor = result;



        // ============ Reflection part ==================
        vec2 whSize = textureSize(tex_mirror, 0);
 
        float texture_width = whSize.x;
        float texture_height = whSize.y;

        float u = gl_FragCoord.x / texture_width;
        float v = gl_FragCoord.y / texture_height;

        v = 1 - v;

        vec2 rfl_uv = gl_FragCoord.xy / vec2(texture_width, texture_height);
        //Check if the tex_mirror is empty (=> black) then we absolutely don't want to merge it 
        //Because it will darken the whole scene

        if(texture(tex_mirror, rfl_uv).rgb != vec3(0.0, 0.0, 0.0) ) {
            vec3 terrainReflected = texture(tex_mirror, rfl_uv).rgb;
            finalColor = vec4(mix(finalColor.xyz, terrainReflected, 0.5), 0.6);
        }

        // ============ Fog part =======================

        if (fogEnabled > 0.5) { // not == 1 to avoid float procession error
            float distance = distance(cameraPos, fragPos);
            float fogAmount = exp(distance * 0.009) - 1;
            fogAmount = clamp(fogAmount, 0, 0.8);
            vec4  fogColor  = vec4(1,1,1,1);
            finalColor = mix( finalColor, fogColor, fogAmount );
        }
     
    }

    color = finalColor;
}
