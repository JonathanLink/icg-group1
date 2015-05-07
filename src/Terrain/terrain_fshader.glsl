#version 330 core

in float my_height;
in float diffuse_component;

out vec3 color;

void main() {
    if (my_height <= 0.39) {
        color = vec3(0.0, 0.0, 0.9) + diffuse_component;
    } else if (my_height <= 0.47) {
        color = vec3(0.9, 0.9, 0.0) + diffuse_component;
    } else if (my_height <= 0.7) {
        color = vec3(0.0, 0.9, 0.0) + diffuse_component;
    } else {
        color = vec3(0.9, 0.9, 0.9) + diffuse_component;
    }

    //color = vec3(height0, height0, height0);
    //color *= diffuseLength * 5.0;
    //color = vec3(1.0, 0.0, 0.0);
}
