#version 330 core

out vec3 color;


// noise
float noise(vec2 pos)
{
    return fract( sin( dot(pos*0.001 ,vec2(24.12357, 36.789) ) ) * 12345.123);  
}


// blur noise
float smooth_noise(vec2 pos)
{
    return   ( noise(pos + vec2(1,1)) + noise(pos + vec2(1,1)) + noise(pos + vec2(1,1)) + noise(pos + vec2(1,1)) ) / 16.0       
           + ( noise(pos + vec2(1,0)) + noise(pos + vec2(-1,0)) + noise(pos + vec2(0,1)) + noise(pos + vec2(0,-1)) ) / 8.0      
           + noise(pos) / 4.0;
}


// linear interpolation
float interpolate_noise(vec2 pos)
{
    float   a, b, c, d;
    
    a = smooth_noise(floor(pos));   
    b = smooth_noise(vec2(floor(pos.x+1.0), floor(pos.y)));
    c = smooth_noise(vec2(floor(pos.x), floor(pos.y+1.0)));
    d = smooth_noise(vec2(floor(pos.x+1.0), floor(pos.y+1.0)));
        
    a = mix(a, b, fract(pos.x));
    b = mix(c, d, fract(pos.x));
    a = mix(a, b, fract(pos.y));
    
    return a;                   
}

float perlin_noise(vec2 pos)
{
    float   n;
    
    n = interpolate_noise(pos*0.0625)*0.5;
    n += interpolate_noise(pos*0.125)*0.25;
    n += interpolate_noise(pos*0.025)*0.225;
    n += interpolate_noise(pos*0.05)*0.0625;
    n += interpolate_noise(pos)*0.03125;
    return n;
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
    float a = 0.8;
    vec2 shift = vec2(100);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
    int NUM_OCTAVES = 7;
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        float noise = noise(x, 5);
        v += a * noise;
        //x = x * 2.0;
        //x = x * 2.0 + shift;
        x = rot * x * 2.0 + shift;
        a *= 0.25;
        x *= lacunarity;
    }
    return v;
}

float fbmOld(vec2 x) {
    float v = 0.0;
    float a = 0.8;
    vec2 shift = vec2(100);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
    int NUM_OCTAVES = 5; //  7
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


float multiFractal(vec2 p, float H, float lacunarity, int octaves, float offset) {
    float value = 1.0;
    for (int i = 0; i < octaves; ++i) {
        value *= noise(p) + offset * pow(lacunarity, -H*i);
        p *= lacunarity;
    }
    return value;
}


void main() {

    /*float c = noise(gl_FragCoord.xy, 5);*/
    
//*********************
    float c = fbm(gl_FragCoord.xy, 1.5);
    //float c = fbmOld(gl_FragCoord.xy);
 //*********************


    /*float n = perlin_noise(gl_FragCoord.xy);
    vec2 p = gl_FragCoord.xy * 0.0020;        
    float c = abs(sin(p.x+n*0.8)*cos(p.y+n*0.6));
    */


    /*float n = perlin_noise(gl_FragCoord.xy);
    vec2 p = gl_FragCoord.xy * 0.0020;        
    float c = abs(sin(p.x+n*0.8)*cos(p.y+n*0.6));
    */

    color = vec3(c);
}


