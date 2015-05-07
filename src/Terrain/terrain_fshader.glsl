#version 330 core

in float my_height;
in float diffuse_component;

uniform sampler2D grassTex;
uniform sampler2D snowTex;
uniform sampler2D sandTex;
uniform sampler2D rockTex;  

uniform mat4 normal_matrix;

out vec3 color;
in vec2 uv;
in vec3 vNormal;

float grassCoeff(float slope) {
    if (abs(slope) >= 0.7) {
        return 0.0;
    } else if (abs(slope) <= 0.3) {
        return 1.0;
    } else {
        return slope * slope;
    }
}

void main() {
    float tilingScale = 10;

    //value ranges from -1 to 1 => 1 means parallell to horizontal
    vec4 normal = normal_matrix * vec4(vNormal, 1);
    //float slope = 1.0 - normal.y;
    float slope = clamp(dot(normal.xyz, vec3(1.0, 0.0, 0.0)), 0.0, 1.0);

    if (my_height <= 0.39) {
    	 color = texture(sandTex, 6 * tilingScale * uv).rgb + diffuse_component;
    } else if (my_height <= 0.85) {
        color = mix(texture(rockTex, tilingScale * uv).rgb, texture(grassTex, tilingScale * uv).rgb, 0.6) + diffuse_component;
    } else {
        color = texture(snowTex, 3 * tilingScale * uv).rgb + diffuse_component;
    }

    /* To debug slope value
    if (abs(slope) > 0.0) {
        color = vec3(1.0,0.0,0.0);
    } else {
        color = vec3(0.0,1.0,0.0);
    }
    */
    //color = vec3(height0, height0, height0);
    //color *= diffuseLength * 5.0;
    //color = vec3(1.0, 0.0, 0.0);
}
