#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Bezier/CameraBezier.h"

class Scene {

enum CameraMode { FLY, FPS, BEZIER };

public:
    Scene(glm::vec3 camera_position, glm::vec3 camera_rotation);
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void cleanUp() = 0;
    void renderScene();
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode); 
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void setSceneWidth(GLuint width);
    void setSceneHeight(GLuint height);
    GLuint getSceneAspectRatio() const;
    glm::vec3 getCameraPosition();
    glm::vec3 getLightPosition();
    void setUniformVariables(GLuint pid, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);
    GLfloat getDeltaTime();
    bool fogEnabled();
    void setCameraBezier(CameraBezier cameraBezier);

protected: 
    glm::mat4 view;
    glm::mat4 projection;
    Camera camera;
    std::vector<bool> keys;
    GLfloat lastX;
    GLfloat lastY;
    bool firstMouse;
    GLfloat deltaTime;
    GLfloat lastTime;

protected:
    void updateFlyCameraPosition();
    virtual void updateFpsCameraPosition() = 0;
    GLuint _sceneWidth;
    GLuint _sceneHeight;
    CameraMode _cameraMode;
    double _lightAngle;
    glm::vec3 _lightPosition;
    bool _fog;
    CameraBezier _cameraBezier;
};

#endif