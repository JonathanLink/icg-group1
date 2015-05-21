#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "pgl/Scene.h"

Scene::Scene(glm::vec3 camera_position, glm::vec3 camera_rotation) : camera(camera_position, camera_rotation) {
    keys.resize(1024, false);
    _cameraMode = FLY;
    _fog = true;

}

void Scene::renderScene() {
    // update time
    deltaTime = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    // update light position
    float radius = 5.0f;
    float xSun = radius * sin(_lightAngle * 180.0/3.14) + 5.0f;
    float zSun = radius * cos(_lightAngle * 180.0/3.14);
    _lightPosition = glm::vec3(xSun, 5.0f, zSun);
    _lightAngle = _lightAngle + 0.01 * deltaTime;

    // update camera
    switch (_cameraMode) {
        case FLY: {
            updateFlyCameraPosition();
            updateInertia();
            view = camera.getViewMatrix();
            break;
        }

        case FPS: {
            updateFpsCameraPosition();
            view = camera.getViewMatrix();
            break;
        }
        case BEZIER: {
            view = _cameraBezier.getViewMatrix();
            break;
        }

        default: {

        }
    }

    // update projection
    //projection = glm::perspective(camera.getZoom(), (GLfloat)getSceneAspectRatio(), 0.1f, 100.0f);
    projection = glm::perspective(45.0f, (GLfloat)getSceneAspectRatio(), 0.1f, 200.0f);

    render();
}

void Scene::keyCallback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mode*/) {
    // camera
    if(action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;  
    }
    // fog
    if(action == GLFW_PRESS && keys[GLFW_KEY_F]) {
        _fog = !_fog;
    }
    // camera mode
    if(action == GLFW_PRESS && keys[GLFW_KEY_1]) {
        _cameraMode = FLY;
    }
    if(action == GLFW_PRESS && keys[GLFW_KEY_2]) {
        _cameraMode = FPS;
    }
    if(action == GLFW_PRESS && keys[GLFW_KEY_3]) {
        _cameraMode = BEZIER;
    }
}

void Scene::mouseCallback(GLFWwindow* /*window*/, double xpos, double ypos) {
    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xOffset = xpos - lastX;
    GLfloat yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.rotate(xOffset, yOffset);
}

void Scene::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.zoom(yoffset);
}

void Scene::setSceneWidth(GLuint width) {
    _sceneWidth = width;
}

void Scene::setSceneHeight(GLuint height) {
    _sceneHeight = height;
}

GLuint Scene::getSceneAspectRatio() const {
    return _sceneWidth / _sceneHeight;
}

void Scene::updateFlyCameraPosition() {
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_S] || keys[GLFW_KEY_A] || keys[GLFW_KEY_D]) {
        _isInerting = false;

        glm::vec3 oldCameraPosition = camera.getPosition();
        if(keys[GLFW_KEY_W]) {
            camera.translate(Camera::FORWARD, deltaTime);
        }
        if(keys[GLFW_KEY_S]) {
            camera.translate(Camera::BACKWARD, deltaTime);
        }
        if(keys[GLFW_KEY_A]) {
            camera.translate(Camera::LEFT, deltaTime);
        }
        if(keys[GLFW_KEY_D]) {
            camera.translate(Camera::RIGHT, deltaTime);
        }

        // Save last direction of the camera
        glm::vec3 newCameraPosition = camera.getPosition();
        _lastDirection = glm::normalize(newCameraPosition - oldCameraPosition);
    }
}

void Scene::updateInertia() {
    if (glm::length(_lastDirection) > 0.0 && !_isInerting) {
        // If camera moved at previous call and we're not already inerting
        _isInerting = true;
        _initialInertionTime = glfwGetTime();
    } else if (_isInerting) {
        GLfloat timeSinceInertionStart = glfwGetTime() - _initialInertionTime;
        if (timeSinceInertionStart <= 2.0) {
            // Compute speed malus, decreasing from 1 to 0 until end of inertion
            GLfloat speedMalus = 1.0 - timeSinceInertionStart / 2.0;
            camera.move(_lastDirection, deltaTime * speedMalus);
        } else {
            _isInerting = false;
            // After inertion, the camera is not moving anymore
            _lastDirection = glm::vec3(0.0, 0.0, 0.0);
        }
    }
}


void Scene::setUniformVariables(GLuint pid, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
    GLuint modelLoc_id = glGetUniformLocation(pid, "model");
    glUniformMatrix4fv(modelLoc_id, 1, GL_FALSE,  glm::value_ptr(model));

    GLuint modelViewLoc_id = glGetUniformLocation(pid, "modelView");
    glm::mat4 modelView = view * model;
    glUniformMatrix4fv(modelViewLoc_id, 1, GL_FALSE,  glm::value_ptr(modelView));

    GLuint mvpLoc_id = glGetUniformLocation(pid, "MVP_matrix");
    glm::mat4 mvp = projection * modelView;
    glUniformMatrix4fv(mvpLoc_id, 1, GL_FALSE,  glm::value_ptr(mvp));

    // light vector
    glm::vec3 lightPos = getLightPosition();
    GLint lightPosLoc = glGetUniformLocation(pid, "lightPos");
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);  

    // light color
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    GLint lightColorLoc = glGetUniformLocation(pid, "lightColor");
    glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);  

    // camera position
    glm::vec3 cameraPos = getCameraPosition();
    GLint cameraPosLoc = glGetUniformLocation(pid, "cameraPos");
    glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);  

    // fog
    GLint fogEnabledLoc = glGetUniformLocation(pid, "fogEnabled");
    int fogValue = (_fog) ? 1 : 0 ;
    glUniform1i(fogEnabledLoc, fogValue);  

}

glm::vec3 Scene::getCameraPosition() {
    return camera.getPosition();
}

glm::vec3 Scene::getLightPosition() {
   return _lightPosition;
}

GLfloat Scene::getDeltaTime() {
    return deltaTime;
}

bool Scene::fogEnabled() {
    return _fog;
}

void Scene::setCameraBezier(CameraBezier cameraBezier) {
    _cameraBezier = cameraBezier;
}
