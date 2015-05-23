#include <iostream>
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"
#include "pgl/Bezier/CameraBezier.h"


CameraBezier::CameraBezier() {
}

void CameraBezier::setHulls(const std::vector<Hull> &cameraHulls, const std::vector<Hull> &lookHulls) {
 
    clear();
    _cameraHulls = cameraHulls;
    _lookHulls = lookHulls;
    _cameraPositionCurve.addHulls(_cameraHulls);
    _cameraLookCurve.addHulls(_lookHulls);
}

void CameraBezier::clear() {
    _cameraPositionCurve.clear();
    _cameraLookCurve.clear();
}

glm::mat4 CameraBezier::getViewMatrix() {

    float openGlTime = glfwGetTime();
    float amplitude = 1.0;
    float period = 16;
    float PI = 3.1415;
    float t = fabs((2.0 * amplitude / PI) * asin(sin(2 * PI / period * openGlTime)));
    glm::vec3 cameraPosition = _cameraPositionCurve.samplePointAtTime(t);
    glm::vec3 cameraLook = _cameraLookCurve.samplePointAtTime(t);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    //std::cout << "t=" << t << " pos: "<< glm::to_string(cameraLook) << std::endl;
    return glm::lookAt(cameraPosition, cameraLook, cameraUp);
}

std::vector<glm::vec3> CameraBezier::getCameraCurvePoints() {
    return _cameraPositionCurve.getVertices();
}

std::vector<glm::vec3> CameraBezier::getLookCurvePoints() {
    return _cameraLookCurve.getVertices();
}


