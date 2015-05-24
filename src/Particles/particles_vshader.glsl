#version 330 core

in vec3 position;
uniform mat4 current_model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * current_model * vec4(position, 1.0);
}
