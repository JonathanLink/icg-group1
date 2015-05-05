#include "pgl/Camera.h"

Camera::Camera(glm::vec3 position) {
    _position = position;
    _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    _yaw = YAW;
    _pitch = PITCH;
    _front = glm::vec3(0.0f, 0.0f, -1.0f);
    _movementSpeed = SPEED;
    _mouseSensitivity = SENSITIVTY;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void Camera::translate(CameraMovement direction, GLfloat deltaTime) {
    GLfloat velocity = _movementSpeed * deltaTime;
    switch(direction) {
        case FORWARD: {
            _position += _front * velocity;
            break;
        }


        case BACKWARD: {
            _position -= _front * velocity;
            break;
        }

        case LEFT: {
            _position -= _right * velocity;
            break;
        }

        case RIGHT: {
            _position += _right * velocity;
            break;
        }

        default: {
            // Do nothing
        }
    }
}

void Camera::rotate(GLfloat xOffset, GLfloat yOffset) {
    xOffset *= _mouseSensitivity;
    yOffset *= _mouseSensitivity;

    _yaw += xOffset;
    _pitch += yOffset;

    if (_pitch > 89.0f) {
        _pitch = 89.0f;
    }
    if (_pitch < -89.0f) {
        _pitch = -89.0f;
    }

    updateCameraVectors();
} 


void Camera::zoom(GLfloat yOffset) {
    if (_zoom >= 1.0f && _zoom <= 45.0f) {
        _zoom -= yOffset;
    }
    if (_zoom <= 1.0f) {
        _zoom = 1.0f;
    }
    if (_zoom >= 45.0f) {
        _zoom= 45.0f;    
    }
}

GLfloat Camera::getZoom() {
    return _zoom;
}

