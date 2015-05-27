#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 uv_coords;
in vec3 fragPos;
in vec3 normal;
in float fragHeight;
in vec2 frag_coord;

uniform sampler2D tex;
uniform sampler2D tex_mirror;
uniform float grid_size;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform mat4 MVP_matrix;
uniform float water_height;
uniform int fogEnabled;

uniform float time;

out vec4 color;

float rand(vec2 c){
    float result = fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
    return result;
}

void main() {

    vec4 textureColor = vec4(0,241.0/255.0, 1.0, 0.7);
    vec4 finalColor = vec4(0,0,0,0);

        // ============ Lightning part ==================

        // Ambient
        float ambientStrength = 0.5f;
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragPos);  
        float diff = max(dot(norm, lightDir), 0.0);
        float cLength = length(uv_coords + 0.2*normal.x);
        vec3 rng = lightColor * 0.2 * cos( time * cLength ) / cLength;
        // Ambient + RNG

        vec4 result = vec4((ambient + rng), 1.0f) * textureColor;
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

            finalColor = vec4(mix(finalColor.xyz, terrainReflected, 0.5), 0.7);
        }

        // ============ Fog part =======================

        if (fogEnabled > 0.5) { // not == 1 to avoid float procession error
            float distance = distance(cameraPos, fragPos);
            float fogAmount = exp(distance * 0.009) - 1;
            fogAmount = clamp(fogAmount, 0, 0.8);
            vec4  fogColor  = vec4(1,1,1,1);
            finalColor = mix( finalColor, fogColor, fogAmount );
        }

    color = finalColor;
}
