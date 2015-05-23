#version 330 core

uniform mat4 MVP_matrix;
uniform vec3 position;

void main() {
    gl_PointSize = 10.0;
    gl_Position = MVP_matrix * vec4(position, 1.0);
}
