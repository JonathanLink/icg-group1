#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"


class Camera {

public:

    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    const GLfloat YAW        = -90.0f;
    const GLfloat PITCH      =  0.0f;
    const GLfloat SPEED      =  3.0f;
    const GLfloat SENSITIVTY =  0.25f;
    const GLfloat ZOOM       =  0.0f;

    Camera(glm::vec3 position);
    glm::mat4 getViewMatrix();
    void translate(CameraMovement direction, GLfloat deltaTime);
    void rotate(GLfloat xOffset, GLfloat yOffset);
    void zoom(GLfloat yOffset);
    GLfloat getZoom();

private:

    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;
    GLfloat _yaw;
    GLfloat _pitch;
    GLfloat _movementSpeed;
    GLfloat _mouseSensitivity;
    GLfloat _zoom;

    void updateCameraVectors();



};

#endif