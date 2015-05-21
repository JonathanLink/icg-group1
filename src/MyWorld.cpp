#include "MyWorld.h"
#include "pgl/FrameBuffer.h"
#include "glm/ext.hpp"

MyWorld::MyWorld(): Scene(glm::vec3(-0.967917f, 20.54413f, -1.45086f),
                          glm::vec3(-22.4157f, 36.1665f, 0.0f)) {
    // Do nothing
}

void MyWorld::init() {
    _skybox.setScene(this);
    _perlin.setScene(this);
    _terrain.setScene(this);
    _water.setScene(this);
    //_fishEye.setScene(this);


    // Draw perlin noise in framebuffer we've just created
    FrameBuffer perlinFrameBuffer = FrameBuffer(FRAME_BUFFER_PERLIN_WIDTH, FRAME_BUFFER_PERLIN_HEIGHT);
    GLuint perlinTextureId = perlinFrameBuffer.initTextureId(GL_R32F); 
    perlinFrameBuffer.bind();
        _perlin.render(view, projection);
    perlinFrameBuffer.unbind();

    _heightMap = new float[FRAME_BUFFER_PERLIN_WIDTH * FRAME_BUFFER_PERLIN_HEIGHT];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, perlinTextureId);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, _heightMap);

    _terrain.setTexture(perlinTextureId);

    // Bezier init
    setCameraBezier(_cameraBezier);
    _bezierEditModeEnabled = false;
    _bezierCurve.setScene(this);
    _bezierCurve2.setScene(this);
    _handle1 = glm::vec3(0,23,-1);
    _handle2 = _handle1;
    _handle3 = _handle1;
    _handle4 = _handle1;
    _selectedHandle = &_handle1;
    buildBezierCurve();


}

void MyWorld::buildBezierCurve() {
    // ***********************************
    // Set camera bezier for path 1
    // ***********************************
    std::vector<Hull> cameraHulls;
    //cameraHulls.push_back(Hull(_handle1, _handle2, _handle3, _handle4));
    cameraHulls.push_back(Hull(_handle1, glm::vec3(0,20,-2), glm::vec3(0,20,-2), glm::vec3(12,42,-1)));
    cameraHulls.push_back(Hull(glm::vec3(12,42,-1), glm::vec3(2,23,-5), glm::vec3(3,10,-3), glm::vec3(3,23,-2)));

    std::vector<Hull> lookHulls;
    lookHulls.push_back(Hull(glm::vec3(0,25,10), glm::vec3(-43,20,10), glm::vec3(43,20,10), glm::vec3(0,20,0)));

    _cameraBezier.clear();
    _cameraBezier.setHulls(cameraHulls, lookHulls);

    _bezierHandles.clear();
    _bezierCurve.setPoints(_cameraBezier.getCameraCurvePoints());
    _bezierHandles.setHandles(cameraHulls, this);


    _bezierCurve2.setPoints(_cameraBezier.getLookCurvePoints());
}

void MyWorld::render() {
    
    //FrameBuffer fishEyeFrameBuffer = FrameBuffer(800, 600);
    //GLuint fishEyeTextureId = fishEyeFrameBuffer.initTextureId(GL_RGB);
    //fishEyeFrameBuffer.bind();
    //    _skybox.render(view, projection);
    //    _terrain.render(view, projection); 
    //fishEyeFrameBuffer.unbind();

    //_fishEye.setTexture(fishEyeTextureId);
    //_fishEye.render(view, projection);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // wireframe

    _skybox.render(view, projection);
    _terrain.render(view, projection);
    _water.render(view, projection);

    _bezierCurve.render(view, projection);
    _bezierHandles.render(view, projection);

    //_bezierCurve2.render(view, projection);
}

void MyWorld::cleanUp() {
    _skybox.cleanUp();
    _terrain.cleanUp();
    _perlin.cleanUp();
    _fishEye.cleanUp();
    _bezierCurve.cleanUp();
    _bezierCurve2.cleanUp();
    _bezierHandles.cleanUp();


}

glm::vec3 delta;
void MyWorld::keyCallback(int key, int /*scancode*/, int action, int /*mode*/) {
    
    // bezier edit mode logic
    float step = 0.5;
    glm::vec3 deltaX(step,0,0);
    glm::vec3 deltaY(0,step,0);
    glm::vec3 deltaZ(0,0,step);

    if(action == GLFW_PRESS && keys[GLFW_KEY_B]) {
        _bezierEditModeEnabled = ! _bezierEditModeEnabled;
        std::cout << "BEZIER EDIT MODE = " << _bezierEditModeEnabled << std::endl;
    }
    
    if (_bezierEditModeEnabled) {

        if(action == GLFW_PRESS && keys[GLFW_KEY_U]) {
             std::cout << "HANDLE 1" << std::endl;
            _selectedHandle = &_handle1;
        } else if(action == GLFW_PRESS && keys[GLFW_KEY_I]) {
             std::cout << "HANDLE 2" << std::endl;
            _selectedHandle = &_handle2;
        } else if(action == GLFW_PRESS && keys[GLFW_KEY_O]) {
            std::cout << "HANDLE 3" << std::endl;
            _selectedHandle = &_handle3;
        } else if(action == GLFW_PRESS && keys[GLFW_KEY_P]) {
            std::cout << "HANDLE 4" << std::endl;
            _selectedHandle = &_handle4;
        }


        if(action == GLFW_PRESS && keys[GLFW_KEY_X]) {
            delta = deltaX;
            std::cout << "BEZIER EDIT SELECTED AXE = X " << std::endl;
        } else if (action == GLFW_PRESS && keys[GLFW_KEY_Z]) {
            delta = deltaY;
            std::cout << "BEZIER EDIT SELECTED AXE = Y " << std::endl;
        } else if(action == GLFW_PRESS && keys[GLFW_KEY_Y]) {
            delta = deltaZ;
            std::cout << "BEZIER EDIT SELECTED AXE = Z " << std::endl;
        }


        if(action == GLFW_PRESS && keys[GLFW_KEY_UP]) {
            *_selectedHandle = *_selectedHandle - delta;
            buildBezierCurve();
        } else if(action == GLFW_PRESS && keys[GLFW_KEY_DOWN]) {
            *_selectedHandle = *_selectedHandle + delta;
            buildBezierCurve();
        }
    }

}

void MyWorld::updateFpsCameraPosition() {

}

float MyWorld::getHeight(int x, int y) {
    return _heightMap[x + FRAME_BUFFER_PERLIN_WIDTH * y];
}