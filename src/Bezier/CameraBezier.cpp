#include <iostream>

#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#include "CameraBezier.h"

 const float PERIOD_STEP = 0.1;

CameraBezier::CameraBezier() {
}

void CameraBezier::setHulls(const std::vector<Hull> &cameraHulls, const std::vector<Hull> &lookHulls) {
 
    clear();
    _cameraHulls = cameraHulls;
    _lookHulls = lookHulls;
    _cameraPositionCurve.addHulls(_cameraHulls);
    _cameraLookCurve.addHulls(_lookHulls);
    _period = 0.1;
}

void CameraBezier::clear() {
    _cameraPositionCurve.clear();
    _cameraLookCurve.clear();
}

glm::mat4 CameraBezier::getViewMatrix() {
    float openGlTime = glfwGetTime() - _initialTime;
    float t = fabs(sin( _period * openGlTime));
    glm::vec3 cameraPosition = _cameraPositionCurve.samplePointAtTime(t);
    glm::vec3 cameraLook = _cameraLookCurve.samplePointAtTime(t);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross( glm::normalize(cameraLook), cameraUp));
    glm::vec3 up = glm::normalize(glm::cross(right, cameraLook));
    return glm::lookAt(cameraPosition, cameraLook, glm::abs(up));
}


glm::vec3 CameraBezier::getPosition() {
    float openGlTime = glfwGetTime() - _initialTime;
    float t = fabs(sin( _period * openGlTime));
    glm::vec3 cameraPosition = _cameraPositionCurve.samplePointAtTime(t);
    return cameraPosition;
}

std::vector<glm::vec3> CameraBezier::getCameraCurvePoints() {
    return _cameraPositionCurve.getVertices();
}

std::vector<glm::vec3> CameraBezier::getLookCurvePoints() {
    return _cameraLookCurve.getVertices();
}

void CameraBezier::setPeriod(float period) {
    _period = period;
}

void CameraBezier::increasePeriod() {
    _period = _period + PERIOD_STEP;
}

void CameraBezier::decreasePeriod() {
    _period = _period - PERIOD_STEP;
}


void CameraBezier::setInitialTime(float initialTime) {
    _initialTime = initialTime;
}
