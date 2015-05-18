#ifndef CAMERA_BEZIER_H
#define CAMERA_BEZIER_H

#include "Hull.h"
#include "Bezier.h"

class CameraBezier {
public:
    CameraBezier();
    glm::mat4 getViewMatrix();
    void setHulls(const std::vector<Hull> &cameraHulls, const std::vector<Hull> &lookHulls);

private:
    Bezier _cameraPositionCurve;
    Bezier _cameraLookCurve;
};

#endif