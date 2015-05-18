#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

class Scene {

enum CameraMode { FLY, FPS, BEZIER };

public:
    Scene();
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
    Camera getCamera();

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

private:
    void updateCameraPosition();
    GLuint _sceneWidth;
    GLuint _sceneHeight;
    CameraMode _cameraMode;
};

#endif