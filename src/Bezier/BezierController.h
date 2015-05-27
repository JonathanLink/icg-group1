#ifndef BEZIER_CONTROLLER_H
#define BEZIER_CONTROLLER_H

#include "../Bezier/Curve/Curve.h"
#include "../Bezier/Curve/Handles/Handles.h"
#include "../Scene/Scene.h"


class BezierController {
public:

    enum BezierCamera {
        BIRD_EYE,
        LAKE,
        PARABOLIC
    };

    BezierController();
    void setScene(Scene *scene);
    void render(const glm::mat4 &view, const glm::mat4 &projection);  
    void cleanUp();
    void keyCallback(int key, int scancode, int action, int mode);

    BezierCamera selectedBezierCamera;    
    bool bezierEditModeEnabled;
    
private:

    void buildBezierCurve();
    void generateSkyViewCurve();
    void generateLakeCurve();
    void generateAroundCurve();
    
    // Bezier 
    Scene *_scene;
    glm::vec3 _delta;
    float _step;
    CameraBezier _cameraBezierTopView;
    CameraBezier _cameraBezierLake;
    Curve _bezierPositionCurve;
    Curve _bezierLookCurve;
    Handles _bezierHandles;
    glm::vec3 *_selectedHandle;
    glm::vec3 _handle1;
    glm::vec3 _handle2;
    glm::vec3 _handle3;
    glm::vec3 _handle4;



};

#endif