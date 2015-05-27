#ifndef CAMERA_BEZIER_H
#define CAMERA_BEZIER_H


#include "Hull.h"
#include "Bezier.h"

class CameraBezier {
public:


    CameraBezier();
    glm::mat4 getViewMatrix();
    void setHulls(const std::vector<Hull> &cameraHulls, const std::vector<Hull> &lookHulls);
    void clear();
    std::vector<glm::vec3> getCameraCurvePoints();
    std::vector<glm::vec3> getLookCurvePoints();
    glm::vec3 getPosition();
    void setPeriod(float period);
    void increasePeriod();
    void decreasePeriod();

    void setInitialTime(float initialTime);

private:
    float _initialTime;
    Bezier _cameraPositionCurve;
    Bezier _cameraLookCurve;
    std::vector<Hull> _cameraHulls;
    std::vector<Hull> _lookHulls;
    float _period;
};

#endif