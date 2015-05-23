#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include "glm/glm.hpp"

class Camera {
public:
    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    const GLfloat SPEED = 10.0f;
    const GLfloat SENSITIVTY = 0.25f;
    const GLfloat ZOOM = 0.0f;

    Camera(const glm::vec3& position, const glm::vec3& rotation);
    glm::mat4 getViewMatrix();
    void move(const glm::vec3& direction, GLfloat deltaTime);
    void translate(CameraMovement direction, GLfloat deltaTime);
    void rotate(GLfloat xOffset, GLfloat yOffset);
    void zoom(GLfloat yOffset);
    GLfloat getZoom();
    glm::vec3 getPosition();
    void setHeight(float height);

private:
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;
    GLfloat _movementSpeed;
    GLfloat _mouseSensitivity;
    GLfloat _zoom;

    void updateCameraVectors();
};

#endif