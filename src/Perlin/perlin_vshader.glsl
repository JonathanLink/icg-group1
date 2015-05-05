#version 330 core

in vec2 position;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
}
