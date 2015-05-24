#version 330 core

in vec3 position;
uniform mat4 current_model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = model * view * projection * vec4(position, 1.0);
}
