#include "pgl/Bezier/CameraBezier.h"


CameraBezier::CameraBezier() {
    _cameraPositionCurve.init();
    _cameraLookCurve.init();
}

void CameraBezier::setHulls(const std::vector<Hull> &cameraHulls, const std::vector<Hull> &lookHulls) {
    _cameraPositionCurve.addHulls(cameraHulls);
    _cameraLookCurve.addHulls(lookHulls);
}


glm::mat4 CameraBezier::getViewMatrix() {
    float openGlTime = glfwGetTime();
    float amplitude = 1.0;
    float period = 16;
    float PI = 3.1415;
    float t = abs((2.0 * amplitude / PI) * asin(sin(2 * PI / period * openGlTime)));
    glm::vec3 cameraPosition = _cameraPositionCurve.samplePointAtTime(t);
    glm::vec3 cameraLook = _cameraLookCurve.samplePointAtTime(t);
    glm::vec3 cameraUp(0.0f, 0.0f, 1.0f);
    return glm::lookAt(cameraPosition, cameraLook, cameraUp);
}







