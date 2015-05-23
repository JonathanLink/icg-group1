#version 330 core

uniform int isLookAtCurve;

out vec3 color;

void main() {
	if (isLookAtCurve < 0.5) {
    	color = vec3(1,0,0);
	} else {
		color = vec3(0,1,0);
	}
}
