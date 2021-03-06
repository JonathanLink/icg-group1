#include <iostream>

#include <GL/glew.h>
#include "glm/ext.hpp"

#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation) {
    _position = position;
    _rotation = rotation;
    _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    _front = glm::vec3(0.0f, 0.0f, -1.0f);
    _movementSpeed = SPEED;
    _mouseSensitivity = SENSITIVTY;
    updateCameraVectors();
}


glm::mat4 Camera::getViewMatrix() {
    /*std::cout << "eye: "<< glm::to_string(_position) << std::endl;
    std::cout << "center: " << glm::to_string(_position + _front) << std::endl;
    std::cout << "_up: " << glm::to_string(_position + _front) << std::endl;*/
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(_rotation.y)) * cos(glm::radians(_rotation.x));
    front.y = sin(glm::radians(_rotation.x));
    front.z = sin(glm::radians(_rotation.y)) * cos(glm::radians(_rotation.x));
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void Camera::move(const glm::vec3& direction, GLfloat deltaTime) {
    GLfloat velocity = _movementSpeed * deltaTime;
    _position += direction * velocity;
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
    //std::cout << "cam pos: x= " << _position.x << " y= " << _position.y << " z = " << _position.z << std::endl;
}

void Camera::rotate(GLfloat xOffset, GLfloat yOffset) {
    xOffset *= _mouseSensitivity;
    yOffset *= _mouseSensitivity;

    _rotation.y += xOffset;
    _rotation.x += yOffset;

    if (_rotation.x > 89.0f) {
        _rotation.x = 89.0f;
    }
    if (_rotation.x < -89.0f) {
        _rotation.x = -89.0f;
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

const glm::vec3& Camera::getPosition() const {
   // std::cout << glm::to_string(_position) << std::endl;
    return _position;
}

void Camera::setHeight(float height) {
    _position.y = height;
}

