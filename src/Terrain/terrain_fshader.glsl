#version 330 core

in float my_height;
in vec3 my_pos;
in vec3 friend_pos_1;
in vec3 friend_pos_2;

out vec3 color;

uniform mat4 normal_matrix;

void main() {
    vec3 normal = normalize(
        normal_matrix *
        vec4(cross(friend_pos_1 - my_pos, friend_pos_2 - my_pos), 0)
    ).xyz;
    vec3 light_pos = vec3(0.0, 0.99, 0.0);
    vec3 light = normalize(my_pos - light_pos);
    float diffuse_coefficient = max(0.0, dot(normal, light));

    if (my_height <= 0.39) {
    	 color = vec3(0.1, 0.01, 1.0) * diffuse_coefficient;
    } else if (my_height <= 0.47) {
        color = vec3(1.0, 1.0, 0.01) * diffuse_coefficient;
    } else if (my_height <= 0.7) {
        color = vec3(0.1, 1.0, 0.01) * diffuse_coefficient;
    } else {
        color = vec3(1.0, 1.0, 1.0) * diffuse_coefficient;
    }

    //color = vec3(height0, height0, height0);
    //color *= diffuseLength * 5.0;
    //color = vec3(1.0, 0.0, 0.0);
}
