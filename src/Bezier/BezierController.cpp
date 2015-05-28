#include "BezierController.h"
#include "../Scene/Scene.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"

/*
Very ugly code. I know it. Lack of time.
*/

BezierController::BezierController() {
   // Do nothing
}

void BezierController::setScene(Scene *scene) {

     // BezierController init
    _scene = scene;
    selectedBezierCamera = BezierController::BIRD_EYE;
    _step = 0.5;
    bezierEditModeEnabled = false;
    _bezierPositionCurve.setScene(_scene);
    _bezierLookCurve.setScene(_scene);

    // CURVE OK
    _handle1 = glm::vec3(-23.299969, 22.899982, 9.999995);
    _handle2 = glm::vec3(-14.100016, 5.499984, -4.300000);
    _handle3 = glm::vec3(-8.099995, 4.999980, -10.399996);
    _handle4 = glm::vec3(13.699999, 35.699963, -3.599999);

    _selectedHandle = &_handle1;
    buildBezierCurve();
   
}

void BezierController::generateSkyViewCurve() {
    std::vector<Hull> cameraHulls;
    cameraHulls.clear();
    cameraHulls.push_back(Hull(glm::vec3(0.0, 65.0, 20.0), glm::vec3(0.0, 65.0, 1.0), glm::vec3(0.0,65.0, -1.0), glm::vec3(0.0, 65.0, -20.0)));
    
    std::vector<Hull> lookHulls;
    lookHulls.clear();
    lookHulls.push_back(Hull(glm::vec3(0.0, 64.9, 20.0), glm::vec3(0.0,  64.6, 1.0), glm::vec3(0.0,  64.6, -1.0), glm::vec3(0.0,  64.9, -20.0)));
    _cameraBezierTopView.setHulls(cameraHulls, lookHulls);
    _scene->setCameraBezier(_cameraBezierTopView);
    _bezierPositionCurve.setPoints(_cameraBezierTopView.getCameraCurvePoints());
    _bezierLookCurve.setIsLookAtCurve(true);
    _bezierLookCurve.setPoints(_cameraBezierTopView.getLookCurvePoints());

    _bezierHandles.setHandles(cameraHulls, _scene);

}

void BezierController::generateLakeCurve() {
    std::vector<Hull> cameraHulls;
    cameraHulls.clear();
    cameraHulls.push_back(Hull(_handle1, _handle2, _handle3, _handle4));
    
    std::vector<Hull> lookHulls;
    lookHulls.clear();
    lookHulls.push_back(Hull(glm::vec3(-25.799969, 20.399982, 25.099995), glm::vec3(-32.500004, 13.299983, 4.400000), glm::vec3(-15.599994, 12.499980, 5.4f), glm::vec3(-12.699999, 15.199974, 8.599999)));

    _cameraBezierLake.setHulls(cameraHulls, lookHulls);
    _scene->setCameraBezier(_cameraBezierLake);

    _bezierPositionCurve.setPoints(_cameraBezierLake.getCameraCurvePoints());
    _bezierLookCurve.setIsLookAtCurve(true);
    _bezierLookCurve.setPoints(_cameraBezierLake.getLookCurvePoints());

    _bezierHandles.setHandles(cameraHulls, _scene);
}


void BezierController::generateAroundCurve() {

    std::vector<Hull> cameraHulls;
    cameraHulls.clear();
    cameraHulls.push_back(Hull(glm::vec3(0, 30, 35), glm::vec3(-10, 120, 1), glm::vec3(-20, 75, -1), glm::vec3(-30, 20, -15)));
    
    std::vector<Hull> lookHulls;
    lookHulls.clear();
    lookHulls.push_back(Hull(glm::vec3(-20, 0, -20), glm::vec3(20, 0, 0), glm::vec3(20, 0, 0), glm::vec3(-20, 0, 20)));
    

    _cameraBezierLake.setHulls(cameraHulls, lookHulls);
    _scene->setCameraBezier(_cameraBezierLake);

    _bezierPositionCurve.setPoints(_cameraBezierLake.getCameraCurvePoints());
    _bezierLookCurve.setIsLookAtCurve(true);
    _bezierLookCurve.setPoints(_cameraBezierLake.getLookCurvePoints());

    _bezierHandles.setHandles(lookHulls, _scene);
}

void BezierController::keyCallback(int key, int scancode, int action, int mode) {

    if (action == GLFW_PRESS && key == GLFW_KEY_V) {
        selectedBezierCamera = (BezierCamera)((selectedBezierCamera + 1) % 3);
        buildBezierCurve();
    }

    glm::vec3 deltaX(_step,0,0);
    glm::vec3 deltaY(0,_step,0);
    glm::vec3 deltaZ(0,0,_step);

    if(action == GLFW_PRESS && key == GLFW_KEY_B) {
        bezierEditModeEnabled = ! bezierEditModeEnabled;

        _scene->_isInertiaEnabled = ! _scene->_isInertiaEnabled;
        // Reset inertia
        _scene->_isInerting = false;
        _scene->_lastDirection = glm::vec3(0.0, 0.0, 0.0);
        std::cout << "BEZIER EDIT MODE = " << bezierEditModeEnabled << std::endl;
    }

    if (bezierEditModeEnabled) {
        if (action == GLFW_PRESS && key == GLFW_KEY_U) {
             std::cout << "HANDLE 1" << std::endl;
            _selectedHandle = &_handle1;
        } else if (action == GLFW_PRESS && key == GLFW_KEY_I) {
             std::cout << "HANDLE 2" << std::endl;
            _selectedHandle = &_handle2;
        } else if (action == GLFW_PRESS && key == GLFW_KEY_O) {
            std::cout << "HANDLE 3" << std::endl;
            _selectedHandle = &_handle3;
        } else if (action == GLFW_PRESS && key == GLFW_KEY_P) {
            std::cout << "HANDLE 4" << std::endl;
            _selectedHandle = &_handle4;
        }

        if (action == GLFW_PRESS && key == GLFW_KEY_X) {
            _delta = deltaX;
            std::cout << "BEZIER EDIT SELECTED AXE = X " << std::endl;
        } else if (action == GLFW_PRESS && key == GLFW_KEY_Z) {
            _delta = deltaY;
            std::cout << "BEZIER EDIT SELECTED AXE = Y " << std::endl;
        } else if (action == GLFW_PRESS && key == GLFW_KEY_Y) {
            _delta = deltaZ;
            std::cout << "BEZIER EDIT SELECTED AXE = Z " << std::endl;
        }

        if (action == GLFW_PRESS  && key == GLFW_KEY_UP) {
            *_selectedHandle = *_selectedHandle - _delta;
            buildBezierCurve();
        } else if (action == GLFW_PRESS  && key == GLFW_KEY_DOWN) {
            *_selectedHandle = *_selectedHandle + _delta;
            buildBezierCurve();
        }

        if (action == GLFW_PRESS  && key == GLFW_KEY_RIGHT) {
            _step += 0.2;
            std::cout << "BEZIER EDIT step = " << _step << std::endl;
        } else if (action == GLFW_PRESS  && key == GLFW_KEY_LEFT) {
            _step -= 0.2;
            if (_step < 0) _step = 0;
            std::cout << "BEZIER EDIT step = " << _step << std::endl;
        }

        if (action == GLFW_PRESS && key == GLFW_KEY_ENTER) {
            std::cout << "camera/lookHulls.push_back(Hull(glm::"<< glm::to_string(_handle1) << ", glm::" << glm::to_string(_handle2) << ", glm::" << glm::to_string(_handle3) << ", glm::" << glm::to_string(_handle4) << "));"<<std::endl;
        }

    }

}


void BezierController::buildBezierCurve() {
    
    switch (selectedBezierCamera) {
        case BIRD_EYE: {
            generateSkyViewCurve();
            break;
        }
        case LAKE: {
            generateLakeCurve();
            break;
        }
        case PARABOLIC: {
            generateAroundCurve();
            break;
        }
    }

}

void BezierController::render(const glm::mat4 &view, const glm::mat4 &projection) {
    _bezierPositionCurve.render(view, projection);
    _bezierLookCurve.render(view, projection);
    _bezierHandles.render(view, projection);
}

void BezierController::cleanUp() {
    _bezierPositionCurve.cleanUp();
    _bezierLookCurve.cleanUp();
    _bezierHandles.cleanUp();
}

