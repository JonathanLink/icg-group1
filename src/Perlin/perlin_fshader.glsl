#version 330 core

out vec3 color;

float rand(vec2 c){
    return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
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
    return mix(x1, x2, xy.y);
}

float fbm(vec2 x) {
    float v = 0.0;
    float a = 0.8;
    vec2 shift = vec2(100);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
    int NUM_OCTAVES = 5;
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        float noise = noise(x, 5);
        v += a * noise;
        //x = x * 2.0;
        //x = x * 2.0 + shift;
        x = rot * x * 2.0 + shift;
        a *= 0.25;
    }
    return v;
}

void main() {
    /*float c = noise(gl_FragCoord.xy, 5);*/
    float c = fbm(gl_FragCoord.xy);

    color = vec3(c);
}


