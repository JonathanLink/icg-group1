
#version 330 core

#define M_PI 3.1415926535897932384626433832795
#define TAU 6.28318530718
#define MAX_ITER 5

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

    vec3 waterNormal = normalize(2.0 * texture(tex_water_normal, uv_coords).rgb - 1.0);
    vec4 textureColor = vec4(texture(tex_water, uv_coords).rgb, 0.7);
    vec4 finalColor = vec4(0.0);

    /*
    CREDIT GOES TO SHADERTOY
    */
    
        float time2 = time * .5+23.0;
        vec2 uv = 8 * uv_coords.xy;
       
        vec2 p = mod(uv*TAU, TAU)-250.0;
        vec2 i = vec2(p);
        float c = 1.0;
        float inten = .005;

        for (int n = 0; n < MAX_ITER; n++) 
        {           
            float t = time2 * (1.0 - (3.5 / float(n+1)));
            i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
            c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
        }
        c /= float(MAX_ITER);
        c = 1.17-pow(c, 1.4);
        vec3 colour = vec3(pow(abs(c), 8.0));
        textureColor = vec4(clamp(colour + vec3(0.0, 0.35, 0.5), 0.0, 1.0), 0.8);
    
    /*
    CREDIT GOES TO SHADERTOY
    */
   
    if (fragHeight >= water_height) { // hide water if not a visible lake
       finalColor = vec4(0.0,0.0,0.0,0.0); 
    } else {

        // ============ Lightning part ==================

        // Ambient
        float ambientStrength = 0.5f;
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse
        vec3 norm = normalize(waterNormal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // Ambient + Diffuse
        vec4 result = vec4((ambient), 1.0f) * textureColor;
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

            finalColor.xyz = mix(finalColor.xyz, terrainReflected, 0.5);
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
