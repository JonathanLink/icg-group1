#version 330 core

layout(location = 0) in vec2 position;

out vec2 uvCoord;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
    uvCoord = (position + vec2(1.0f, 1.0f)) * 0.5f;
}
