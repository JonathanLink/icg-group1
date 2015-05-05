#version 330 core

layout (location = 0) in vec2 position; 

out float diffuse_coefficient;
out float my_height;

uniform mat4 MVP;
uniform sampler2D tex;
uniform mat4 normal_matrix;

void main() {
	// UV textures in [0,1] whereas OpenGL [-1, 1]
    vec2 my_uv_coords = (position + vec2(1.0, 1.0)) * 0.5;
	float local_height = texture(tex, my_uv_coords).r;
	vec3 local_pos_3d = vec3(position.x, local_height, -position.y);

	const vec2 size = vec2(2.0,0.0);
	const ivec3 offset = ivec3(-1, 0, 1);
	float s01 = textureOffset(tex, my_uv_coords, offset.xy).r;
	float s21 = textureOffset(tex, my_uv_coords, offset.zy).r;
	float s10 = textureOffset(tex, my_uv_coords, offset.yx).r;
	float s12 = textureOffset(tex, my_uv_coords, offset.yz).r;
    vec3 va = normalize(vec3(size.xy, s21 - s01));
    vec3 vb = normalize(vec3(size.yx, s12 - s10));
	vec3 vertex_normal = vec4(cross(va, vb), 0).xyz;

	vec3 diffuse_reflection_normal = normalize(
		normal_matrix *
		vec4(vertex_normal.x, vertex_normal.y, vertex_normal.z, 0.0)
	).xyz;

	vec3 light_pos = vec3(0.0, 0.99, 0.0);
	vec3 light_direction = normalize(local_pos_3d - light_pos);
	diffuse_coefficient = max(0.0, dot(diffuse_reflection_normal, light_direction));

	my_height = local_height;
    gl_Position = MVP * vec4(local_pos_3d, 1.0);
}
