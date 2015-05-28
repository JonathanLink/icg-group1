#version 330 core

out vec3 color;

uniform float _lacunarity; // 1.5
uniform float _freq; // 5
uniform int _octaves; // 5
uniform float _amplitude; // 0.8
uniform float _x; // 0.8

// noise
float noise(vec2 pos)
{
    return fract( sin( dot(pos*0.001 ,vec2(24.12357, 36.789) ) ) * 12345.123);  
}



float rand(vec2 c){
    float result = fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
    return result;
}

float noise(vec2 p, float freq ){
    float PI = 3.14159265358979323846;
    float unit = 600/freq;
    vec2 ij = floor(p/unit);
    vec2 xy = mod(p,unit)/unit;
    xy = 0.5 * (1.0 - cos(PI*xy));
    float a = rand((ij+vec2(0.,0.)));
    float b = rand((ij+vec2(1.,0.)));
    float c = rand((ij+vec2(0.,1.)));
    float d = rand((ij+vec2(1.,1.)));
    float x1 = mix(a, b, xy.x);
    float x2 = mix(c, d, xy.x);
    float result = mix(x1, x2, xy.y);
    return result;
}

float fbm(vec2 x,float lacunarity) {
    float v = 0.0;
    float a = _amplitude;
    vec2 shift = vec2(100);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.5));
    int NUM_OCTAVES = _octaves;
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        float noise = noise(x, _freq);
        v += a * noise;
        //x = x * 2.0;
        //x = x * 2.0 + shift;
        x = rot * x * 2.0 + shift;
        a *= 0.25;
        x *= lacunarity;
    }
    return v;
}


float multiFractal(vec2 p, float H, float lacunarity, int octaves, float offset) {
    float value = 1.0;
    for (int i = 0; i < octaves; ++i) {
        value *= noise(p) + offset * pow(lacunarity, -H*i);
        p *= lacunarity;
    }
    return value;
}


void main() {


    float c = fbm(gl_FragCoord.xy, _lacunarity);

    color = vec3(c);
}




