#version 330 core

layout (location = 0) in vec2 position; 

out float my_height;
out vec3 my_pos;
out vec3 friend_pos_1;
out vec3 friend_pos_2;

uniform mat4 MVP;
uniform sampler2D tex;

void main() {
	// UV textures in [0,1] whereas OpenGL [-1, 1]
    vec2 my_uv_coords = (position + vec2(1.0, 1.0)) * 0.5;

	float local_height = texture(tex, my_uv_coords).r;
	vec3 local_pos_3d = vec3(position.x, local_height, -position.y);

	float delta = 0.1;
	vec2 east_move = vec2(delta, delta);
	vec2 west_move = -east_move;
	float friend_height_west = texture(tex, my_uv_coords + west_move).r;
	float friend_height_east = texture(tex, my_uv_coords + east_move).r;

    gl_Position = MVP * vec4(local_pos_3d, 1.0);
	my_height = local_height;
    my_pos = local_pos_3d;
    friend_pos_1 = vec3(local_pos_3d.x + east_move.x, friend_height_east, local_pos_3d.z + east_move.y);
	friend_pos_2 = vec3(local_pos_3d.x + west_move.x, friend_height_west, local_pos_3d.z + west_move.y);
}
