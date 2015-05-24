#version 330 core

in vec3 position;
out float height;

uniform mat4 current_model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    height = position.y;
    gl_Position = projection * view * current_model * vec4(position, 1.0);
}
