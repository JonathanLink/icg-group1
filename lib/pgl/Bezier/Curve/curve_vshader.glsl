#version 330 core

in vec3 position;
uniform mat4 MVP_matrix;

void main() {
    gl_Position = MVP_matrix * vec4(position, 1.0);
}

