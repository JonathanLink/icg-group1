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
    if (abs(local_slope) >= 0.7) {
        return 0.0;
    } else if (abs(local_slope) <= 0.3) {
        return 1.0;
    } else {
        return local_slope * local_slope * local_slope;
    }
}

/*vec3 getNormal(vec2 fragPos) {
    
    float delta = 1.0 / grid_size;
    // Create 2D vectors
    vec2 north2D = vec2(fragPos.x, fragPos.y + delta);
    vec2 south2D = vec2(fragPos.x, fragPos.y - delta);
    vec2 east2D = vec2(fragPos.x + delta, fragPos.y);
    vec2 west2D = vec2(fragPos.x - delta, fragPos.y);

    // Look up height of neighbors in height map
    float northHeight = texture(tex, north2D).r;
    float southHeight = texture(tex, south2D).r;
    float eastHeight = texture(tex, east2D).r;
    float westHeight = texture(tex, west2D).r;

    // Create 3D vectors
    vec3 north3D = vec3(north2D.x, northHeight, north2D.y);
    vec3 south3D = vec3(south2D.x, southHeight, south2D.y);
    vec3 east3D = vec3(east2D.x, eastHeight, east2D.y);
    vec3 west3D = vec3(west2D.x, westHeight, west2D.y);

    // First triangle normal (triangle: north-west-south)
    vec3 v = north3D - west3D;
    vec3 w = south3D - west3D;
    vec3 normalA = normalize(cross(v, w));

    // Second triangle normal (triangle: north-east-south)
    vec3 v2 = north3D - east3D;
    vec3 w2 = south3D - east3D;
    vec3 normalB = normalize(cross(v2, w2));

    // Compute average normal vector
    vec3 avgNormal = (normalA + normalB) / 2.0f;
    //return vec3(0.0f, 0.0f, 1.0f);
    return normalize(normalA);
    

    
}*/

void main() {

    //vec3 normal2 = getNormal(gl_FragCoord.xy);

    // ============ Texturing part ==================
    float tilingScalaSand = 30;
    float tilingScaleGrassRock = 10;
    float tilingScaleSnow = 60;

    float angle = abs( acos(dot(normal, vec3(0.0f, -1.0f, 0.0f))) );
    vec3 textureColor;
    if (fragHeight <= 0.39) { // sand
         textureColor = texture(sandTex, 6 * tilingScalaSand * uv_coords).rgb;
         //textureColor = vec3(1.0f,0.0f,0.0f);
    } else if (fragHeight <= 0.8) { // rock
        textureColor = mix(texture(rockTex, tilingScaleGrassRock * uv_coords).rgb, texture(grassTex, tilingScaleGrassRock * uv_coords).rgb, grassCoeff(1.0 - angle));
        //textureColor = vec3(0.0f,1.0f,0.0f);
    } else { // snow
        textureColor = texture(snowTex, 3 * tilingScaleSnow * uv_coords).rgb ;
        //textureColor = vec3(0.0f,0.0f,1.0f);
    }
    
     // ============ Lightning part ==================

    // Ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Ambient + Diffuse
    vec3 result = (ambient + diffuse) * textureColor;
    color = vec4(result, 1.0f);

    //color = vec4(mod(angle,2.0*M_PI) /(2.0*M_PI), mod(angle,2.0*M_PI) /(2.0*M_PI), mod(angle,2.0*M_PI) /(2.0*M_PI), 1.0f);
    //vec2 relFragPos = (fragPos.xz + vec2(1.0, 1.0)) * 0.5;
    //color = texture(tex, relFragPos);

    
}
