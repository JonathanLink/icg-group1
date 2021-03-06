#version 330 core

layout (location = 0) in vec2 position;

out vec2 uv_coords;
out vec3 fragPos;
out vec3 normal;
out float fragHeight;
out vec2 frag_coord;

uniform mat4 model;
uniform mat4 MVP_matrix;
uniform sampler2D tex;
uniform sampler2D tex_mirror;
uniform float grid_size;
uniform float water_height;
uniform float time;

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

    fragHeight = texture(tex, local_uv_coords).r;

    uv_coords = local_uv_coords;

    float CONST_PI = 3.1415926535897932384626433832795;
    float timeValue = time;

    float amplitude = 0.005;
    float spatialFrequency = 4 * 2.0 * CONST_PI;
    float timeFrequency = 0.3 * 2.0 * CONST_PI;
    float phase = CONST_PI / 4.0 + local_uv_coords.x + local_uv_coords.y;

    float heightSine = amplitude * sin(spatialFrequency * (local_uv_coords.x + local_uv_coords.y) +
     timeFrequency * timeValue);

    float heightWater = amplitude - amplitude * sin(spatialFrequency * sqrt(local_uv_coords.x * local_uv_coords.x +
     local_uv_coords.y * local_uv_coords.y) + timeFrequency * timeValue);


    normal = getNormal(local_uv_coords);
    vec3 local_pos_3d = vec3(position.x, water_height - heightWater, position.y); // 0.37 initially for the height
    gl_Position = MVP_matrix * vec4(local_pos_3d, 1.0);
    fragPos = vec3(model * vec4(local_pos_3d, 1.0f));

    frag_coord = vec2(0.5* (gl_Position.x / gl_Position.w + 1.0), 0.5* (gl_Position.y / gl_Position.w + 1.0) );

}




