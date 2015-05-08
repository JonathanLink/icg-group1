#version 330 core

in float my_height;
in float diffuse_component;
in vec2 uv_coords;
in float slope;

uniform sampler2D grassTex;
uniform sampler2D snowTex;
uniform sampler2D sandTex;
uniform sampler2D rockTex;

out vec3 color;

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
    float tilingScalaSand = 30;
    float tilingScaleGrassRock = 10;
    float tilingScaleSnow = 60;

    if (my_height <= 0.39) {
    	 color = texture(sandTex, 6 * tilingScalaSand * uv_coords).rgb + diffuse_component;
    } else if (my_height <= 0.8) {
        color = mix(texture(rockTex, tilingScaleGrassRock * uv_coords).rgb, texture(grassTex, tilingScaleGrassRock * uv_coords).rgb, grassCoeff(1.0 - slope)) + diffuse_component;
     } else {
        color = texture(snowTex, 3 * tilingScaleSnow * uv_coords).rgb + diffuse_component;
    }
}
