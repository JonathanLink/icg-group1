#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 uvCoord;

uniform sampler2D tex;

out vec4 color;

void main() {

	/*float aperture = 178.0;
	float apertureHalf = 0.5 * aperture * (M_PI / 180.0);
	float maxFactor = sin(apertureHalf);
	vec2 uv;
	vec2 xy = 2.0 * uvCoord.xy - 1.0;
	float d = length(xy);

	if (d < (2.0-maxFactor)) {
		d = length(xy * maxFactor);
		float z = sqrt(1.0 - d * d);
		float r = atan(d, z) / M_PI;
		float phi = atan(xy.y, xy.x);

		uv.x = r * cos(phi) + 0.5;
		uv.y = r * sin(phi) + 0.5;

		color = texture(tex, uv);
	} else {
		uv = uvCoord.xy;
		color = vec4(0,0,0,1);
	}*/

	/*float b  = 0.1;
    const float LOG2 = 1.442695;
    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = exp2( -b *  b * z * z * LOG2 );
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3  fogColor  = vec3(1,1,1);

    color = vec4(mix(fogColor, texture(tex, uvCoord).rgb, fogFactor ), 1.0f);
    */

	

    //color = 1 - texture(tex, uvCoord);
}  