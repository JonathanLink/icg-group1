#version 330 core

layout (location = 0) in vec2 position;

out float diffuse_component;
out float my_height;

out vec2 uv;
out vec3 vNormal;

uniform mat4 MVP;
uniform sampler2D tex;
uniform float grid_size;

void main() {
	// UV textures in [0, 1] whereas OpenGL [-1, 1]
    vec2 my_uv_coords = (position + vec2(1.0, 1.0)) * 0.5;

    // Compute 3D position of current vertex by looking up height in height map
	float local_height = texture(tex, my_uv_coords).r;
	vec3 local_pos_3d = vec3(position.x, local_height, -position.y);

	// Find 2D position of neighbors (1 texel distance)
	float heightMapWidth = grid_size;
	float heightMapHeight = grid_size;
	vec2 north_2d = vec2(my_uv_coords.x, my_uv_coords.y + 1.0 / heightMapHeight);
	vec2 west_2d = vec2(my_uv_coords.x - 1.0 / heightMapWidth, my_uv_coords.y);
	vec2 south_2d = vec2(my_uv_coords.x, my_uv_coords.y - 1.0 / heightMapHeight);

	// Look up height of neighbors in height map
	float north_height = texture(tex, north_2d).r;
	float west_height = texture(tex, west_2d).r;
	float south_height = texture(tex, south_2d).r;

	// Generate 3D position of neighbors using the height we just found
	vec3 west_3d = vec3(west_2d.x, west_height, west_2d.y);
	vec3 north_3d = vec3(north_2d.x, north_height, north_2d.y);
	vec3 south_3d = vec3(south_2d.x, south_height, south_2d.y);

	// Find a small plane on the surface by creating two vectors
	// defined by neighbors
	vec3 south_to_north = north_3d - south_3d;
	vec3 south_to_west = west_3d - south_3d;

	// Compute normal using cross product of two vectors of the small plane
	vec3 vertex_normal = normalize(cross(south_to_north, south_to_west));

	// Compute vector going from light to surface
	vec3 light_pos = vec3(5.0, 5.0, 0.0);
	vec3 light_direction = normalize(local_pos_3d - light_pos);

	// Compute diffuse component using scalar product between light direction
	// and surface normal
	float diffuse_coefficient = - 0.5;
	diffuse_component = diffuse_coefficient * max(0.0, dot(vertex_normal, light_direction));

	my_height = local_height;
    gl_Position = MVP * vec4(local_pos_3d, 1.0);
}
