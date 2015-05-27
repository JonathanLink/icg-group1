#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"

#include "Camera.h"
#include "../Bezier/CameraBezier.h"

class Scene {

enum CameraMode { FLY, FPS, BEZIER };

public:
    Scene(const glm::vec3& camera_position, const glm::vec3& camera_rotation);
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void cleanUp() = 0;
    virtual void keyCallback(int key, int scancode, int action, int mode) = 0;
    void renderScene();
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode); 
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void setSceneWidth(GLuint width);
    void setSceneHeight(GLuint height);
    float getSceneAspectRatio() const;
    glm::vec3 getCameraPosition();
    glm::vec3 getLightPosition();
    void setUniformVariables(GLuint pid, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);
    GLfloat getDeltaTime();
    bool fogEnabled();
    void setCameraBezier(CameraBezier cameraBezier);

    glm::float1 getReflectTime();
    void setReflectTime(glm::float1 t);

protected: 
    Camera _camera;

    glm::mat4 _view;
    glm::mat4 _projection;
    std::vector<bool> _keys;
    GLfloat _lastX;
    GLfloat _lastY;
    bool _firstMouse;
    GLfloat _deltaTime;
    GLfloat _lastTime;

protected:
    void updateFlyCameraPosition();
    virtual void updateFpsCameraPosition() = 0;
    bool _isInertiaEnabled = true;
    void updateInertia();
    bool _isInerting = false;
    GLfloat _initialInertionTime = 0.0;
    glm::vec3 _lastDirection;

    GLfloat _jumpStartTime = 0.0;
    GLfloat _jumpStartHeight = 0.0;
    bool _hasJumped = false;

    GLuint _sceneWidth;
    GLuint _sceneHeight;
    CameraMode _cameraMode;
    double _lightAngle;
    glm::vec3 _lightPosition;
    bool _fog;
    CameraBezier _cameraBezier;
    glm::float1 _reflectTime;
};

#endif