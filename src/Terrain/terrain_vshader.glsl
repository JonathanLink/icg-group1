#version 330 core

layout (location = 0) in vec2 position; 


uniform mat4 MVP;
uniform sampler2D tex;

out vec2 uv;


void main() {

	uv = (position + vec2(1.0, 1.0)) * 0.5;
	float height = texture(tex, uv).r;
	vec3 pos_3d = vec3(position.x, height, -position.y);
    gl_Position = MVP * vec4(pos_3d, 1.0); 


}