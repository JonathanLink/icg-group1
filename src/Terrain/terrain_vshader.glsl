#version 330 core

layout (location = 0) in vec2 position;

out vec2 uv_coords;
out vec3 fragPos;
out float fragHeight;

uniform mat4 model;
uniform mat4 MVP_matrix;
uniform sampler2D tex;
uniform float grid_size;

void main() {
	
	vec2 local_uv_coords = (position + vec2(1.0, 1.0)) * 0.5;

	uv_coords = local_uv_coords;
	fragHeight = texture(tex, local_uv_coords).r;

	vec3 local_pos_3d = vec3(position.x, fragHeight, -position.y);
    gl_Position = MVP_matrix * vec4(local_pos_3d, 1.0);
    fragPos = vec3(model * vec4(local_pos_3d, 1.0f));

}




