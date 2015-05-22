#version 330 core

layout (location = 0) in vec2 position;

out vec2 uv_coords;
out vec3 fragPos;
out float fragHeight;
out vec3 normal;

uniform mat4 model;
uniform mat4 MVP_matrix;
uniform sampler2D tex;
uniform float grid_size;
uniform int isReflected;
uniform float water_height;

vec3 getNormal(vec2 pos) {
    
    float delta = 1.0/grid_size;
    // Create 2D vectors
    vec2 north2D = vec2(pos.x, pos.y + delta);
    vec2 south2D = vec2(pos.x, pos.y - delta);
    vec2 east2D = vec2(pos.x + delta, pos.y);
    vec2 west2D = vec2(pos.x - delta, pos.y);

    // Look up height of neighbors in height map
    float northHeight = texture(tex, north2D).r;
    float southHeight = texture(tex, south2D).r;
    float eastHeight = texture(tex, east2D).r;
    float westHeight = texture(tex, west2D).r;

    // Create 3D vectors
    vec3 north3D = vec3(north2D.x, northHeight, north2D.y);
    vec3 south3D = vec3(south2D.x, southHeight, south2D.y);
    vec3 east3D = vec3(east2D.x, eastHeight, east2D.y);
    vec3 west3D = vec3(west2D.x, westHeight, west2D.y);


    vec3 south_to_north = north3D - south3D;
	vec3 south_to_west = west3D - south3D;

	return normalize(cross(south_to_north, south_to_west));
}

void main() {

	vec2 local_uv_coords = (position + vec2(1.0, 1.0)) * 0.5;

	uv_coords = local_uv_coords;
	fragHeight = texture(tex, local_uv_coords).r;
	normal = getNormal(local_uv_coords);

	vec3 local_pos_3d = vec3(position.x, fragHeight, position.y);
    gl_Position = MVP_matrix * vec4(local_pos_3d, 1.0);

    fragPos = vec3(model * vec4(local_pos_3d, 1.0f));
}




