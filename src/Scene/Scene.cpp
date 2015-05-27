#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../Constants/Constants.h"

#include "Scene.h"

Scene::Scene(const glm::vec3& camera_position, const glm::vec3& camera_rotation) : _camera(camera_position, camera_rotation) {
    _keys.resize(1024, false);
    _cameraMode = FLY;
    _fog = false;
}

void Scene::renderScene() {
    // update time
    _deltaTime = glfwGetTime() - _lastTime;
    _lastTime = glfwGetTime();

    // update light position
    float radius = Constants::TERRAIN_SCALE;
    float xSun = radius * sin(_lightAngle * 180.0/3.14) + radius;
    float zSun = radius * cos(_lightAngle * 180.0/3.14) - 0.0;
    _lightPosition = glm::vec3(xSun, Constants::TERRAIN_SCALE, zSun);
//    _lightAngle = _lightAngle + 0.01 * _deltaTime;

    // update camera
    switch (_cameraMode) {
        case FLY: {
            updateFlyCameraPosition();
            if (_isInertiaEnabled) {
                updateInertia();
            }
            _view = _camera.getViewMatrix();
            break;
        }

        case FPS: {
            updateFpsCameraPosition();
            _view = _camera.getViewMatrix();
            break;
        }

        case BEZIER: {
            _view = _cameraBezier.getViewMatrix();
            break;
        }

        default: {
            // Do nothing
        }
    }

    // update projection
    //projection = glm::perspective(camera.getZoom(), (GLfloat)getSceneAspectRatio(), 0.1f, 100.0f);
    _projection = glm::perspective(45.0f, (GLfloat)getSceneAspectRatio(), 0.1f, 200.0f);

    render();
}

void Scene::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // camera
    if(action == GLFW_PRESS) {
        _keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        _keys[key] = false;
    }

    // fog
    if(action == GLFW_PRESS && _keys[GLFW_KEY_F]) {
        _fog = !_fog;
    }

    // camera mode
    if(action == GLFW_PRESS && _keys[GLFW_KEY_C]) {
        _cameraMode = (CameraMode)((_cameraMode + 1) % 3);
        _cameraBezier.setInitialTime(glfwGetTime());
    }

    keyCallback(key, scancode, action, mode);

}

void Scene::mouseCallback(GLFWwindow* /*window*/, double xpos, double ypos) {
    if(_firstMouse) {
        _lastX = xpos;
        _lastY = ypos;
        _firstMouse = false;
    }

    GLfloat xOffset = xpos - _lastX;
    GLfloat yOffset = _lastY - ypos;
    _lastX = xpos;
    _lastY = ypos;

    _camera.rotate(xOffset, yOffset);
}

void Scene::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    _camera.zoom(yoffset);
}

void Scene::setSceneWidth(GLuint width) {
    _sceneWidth = width;
}

void Scene::setSceneHeight(GLuint height) {
    _sceneHeight = height;
}

float Scene::getSceneAspectRatio() const {
    return ((float)_sceneWidth) / ((float)_sceneHeight);
}

void Scene::updateFlyCameraPosition() {
    if (_keys[GLFW_KEY_W] || _keys[GLFW_KEY_S] || _keys[GLFW_KEY_A] || _keys[GLFW_KEY_D]) {
        _isInerting = false;

        glm::vec3 oldCameraPosition = _camera.getPosition();
        if(_keys[GLFW_KEY_W]) {
            _camera.translate(Camera::FORWARD, _deltaTime);
        }
        if(_keys[GLFW_KEY_S]) {
            _camera.translate(Camera::BACKWARD, _deltaTime);
        }
        if(_keys[GLFW_KEY_A]) {
            _camera.translate(Camera::LEFT, _deltaTime);
        }
        if(_keys[GLFW_KEY_D]) {
            _camera.translate(Camera::RIGHT, _deltaTime);
        }

        // Save last direction of the camera
        glm::vec3 newCameraPosition = _camera.getPosition();
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
            _camera.move(_lastDirection, _deltaTime * speedMalus);
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

    GLuint viewLoc_id = glGetUniformLocation(pid, "view");
    glUniformMatrix4fv(viewLoc_id, 1, GL_FALSE,  glm::value_ptr(view));

    GLuint projectionLoc = glGetUniformLocation(pid, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

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
    //glm::vec3 lightColor = glm::vec3(240.0/255.0, 205/255.0, 102./255.0);
    //glm::vec3 lightColor = glm::vec3(0.3,0.3,0.3);
    GLint lightColorLoc = glGetUniformLocation(pid, "lightColor");
    glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);  

    // camera position
    glm::vec3 cameraPos;
    if (_cameraMode == BEZIER) {
        cameraPos = _cameraBezier.getPosition();
    } else {
        cameraPos = getCameraPosition();
    }
    GLint cameraPosLoc = glGetUniformLocation(pid, "cameraPos");
    glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);  

    // fog
    GLint fogEnabledLoc = glGetUniformLocation(pid, "fogEnabled");
    int fogValue = (_fog) ? 1 : 0 ;
    glUniform1i(fogEnabledLoc, fogValue);  

}

glm::vec3 Scene::getCameraPosition() {
    return _camera.getPosition();
}

glm::vec3 Scene::getLightPosition() {
   return _lightPosition;
}

GLfloat Scene::getDeltaTime() {
    return _deltaTime;
}

bool Scene::fogEnabled() {
    return _fog;
}

void Scene::setCameraBezier(CameraBezier cameraBezier) {
    _cameraBezier = cameraBezier;
}

glm::float1 Scene :: getReflectTime() {
    return _reflectTime;
}

void Scene :: setReflectTime(glm::float1 t) {
    _reflectTime = t;
}
