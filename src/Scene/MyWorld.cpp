#include "../Constants/Constants.h"
#include "MyWorld.h"

MyWorld::MyWorld(): Scene(glm::vec3(-0.967917f, 20.54413f, -1.45086f),
                          glm::vec3(-22.4157f, 36.1665f, 0.0f)),
                    _terrainReflectFB(800, 600) {
    // Do nothing
}

void MyWorld::init() {
    _skybox.setScene(this);
    _perlin.setScene(this);
    _terrain.setScene(this);
    _water.setScene(this);
    _cube.setScene(this);
    //_fishEye.setScene(this);

    // Draw perlin noise in framebuffer we've just created
    FrameBuffer perlinFrameBuffer = FrameBuffer(FRAME_BUFFER_PERLIN_WIDTH, FRAME_BUFFER_PERLIN_HEIGHT);
    GLuint perlinTextureId = perlinFrameBuffer.initTextureId(GL_R32F); 
    perlinFrameBuffer.bind();
        _perlin.render(view, projection);
    perlinFrameBuffer.unbind();

    // Save height map in memory
    _heightMap = new float[FRAME_BUFFER_PERLIN_WIDTH * FRAME_BUFFER_PERLIN_HEIGHT];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, perlinTextureId);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, _heightMap);

    _water.setTexturePerlin(perlinTextureId);
    _terrain.setTexture(perlinTextureId);

    // Bezier init
    _step = 0.5;
    _bezierEditModeEnabled = false;
    _bezierPositionCurve.setScene(this);
    _bezierLookCurve.setScene(this);
    _handle1 = glm::vec3(-22.000000, 15.000002, 11.400002);
    _handle2 =  glm::vec3(-26.000000, 11.000000, -0.200000);
    _handle3 =  glm::vec3(-5.400002, 10.400000, -9.000000);
    _handle4 = glm::vec3(4.200001, 18.399998, -0.799999);
    _selectedHandle = &_handle1;
    buildBezierCurve();
}

void MyWorld::generateSkyViewCurve() {
     // curve TOP
    std::vector<Hull> cameraHulls;
    cameraHulls.clear();
    cameraHulls.push_back(Hull(glm::vec3(0.0, 65.0, 20.0), glm::vec3(0.0, 65.0, 1.0), glm::vec3(0.0,65.0, -1.0), glm::vec3(0.0, 65.0, -20.0)));
    
    std::vector<Hull> lookHulls;
    lookHulls.clear();
    lookHulls.push_back(Hull(glm::vec3(0.0, 64.6, 20.0), glm::vec3(0.0,  64.6, 1.0), glm::vec3(0.0,  64.6, -1.0), glm::vec3(0.0,  64.6, -20.0)));

    _cameraBezierTopView.setHulls(cameraHulls, lookHulls);
    setCameraBezier(_cameraBezierTopView);

    _bezierPositionCurve.setPoints(_cameraBezierTopView.getCameraCurvePoints());
    _bezierLookCurve.setIsLookAtCurve(true);
    _bezierLookCurve.setPoints(_cameraBezierTopView.getLookCurvePoints());

    _bezierHandles.setHandles(cameraHulls, this);

}

void MyWorld::generateLakeCurve() {
     // curve LAKE
    std::vector<Hull> cameraHulls;
    cameraHulls.clear();
    cameraHulls.push_back(Hull(glm::vec3(-26.299969, 16.399982, 19.099995), glm::vec3(-26.000004, 10.799983, 1.400000), glm::vec3(-11.599994, 9.999980, -9.399996), glm::vec3(3.199999, 12.199974, -4.099999)));

    std::vector<Hull> lookHulls;
    lookHulls.clear();
    //lookHulls.push_back(Hull(glm::vec3(-22.000000, 15.000002, 11.400002), glm::vec3(-26.000000, 11.000000, -0.200000), glm::vec3(-5.400002, 10.400000, -9.000000), glm::vec3(4.200001, 18.399998, -0.799999)));
    lookHulls.push_back(Hull(glm::vec3(-22.000000, 15.000002, 11.400002), glm::vec3(-26.000000, 11.000000, -0.200000), glm::vec3(-5.400002, 9.900000, -9.000000), glm::vec3(4.200001, 11.899998, -0.799999)));


    _cameraBezierLake.setHulls(cameraHulls, lookHulls);
    setCameraBezier(_cameraBezierLake);

    _bezierPositionCurve.setPoints(_cameraBezierLake.getCameraCurvePoints());
    _bezierLookCurve.setIsLookAtCurve(true);
    _bezierLookCurve.setPoints(_cameraBezierLake.getLookCurvePoints());

    _bezierHandles.setHandles(lookHulls, this);
}

void MyWorld::buildBezierCurve() {
    
// decomment which bezier path you want to visualize  
//generateSkyViewCurve();
  generateLakeCurve();  

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
    
    //Draw terrain in framebuffer for water reflection
    GLuint terrainReflectTextureId = _terrainReflectFB.initTextureId(GL_RGB);
    _terrainReflectFB.bind();
        _terrain.setReflection(true);
        _terrain.render(view, projection);
        _terrain.setReflection(false);
    _terrainReflectFB.unbind();
    _water.setTextureMirror(terrainReflectTextureId);

    _skybox.render(view, projection);
    _terrain.render(view, projection);
    _water.render(view, projection);

     _terrainReflectFB.cleanUp();

    _bezierPositionCurve.render(view, projection);
    _bezierLookCurve.render(view, projection);
    _bezierHandles.render(view, projection);



    //_cube.render(view, projection);
}

void MyWorld::cleanUp() {
    _skybox.cleanUp();
    _terrain.cleanUp();
    _perlin.cleanUp();
    _fishEye.cleanUp();
    _bezierPositionCurve.cleanUp();
    _bezierLookCurve.cleanUp();
    _bezierHandles.cleanUp();
}

// This method is ugly and i know it!
void MyWorld::keyCallback(int key, int /*scancode*/, int action, int /*mode*/) {
    
    // *********************************
    // ****bezier ajust speed ****
    // *********************************

    if (action == GLFW_PRESS && keys[GLFW_KEY_K]) {
        _cameraBezier.increasePeriod();
    }
    if (action == GLFW_PRESS && keys[GLFW_KEY_L]) {
        _cameraBezier.decreasePeriod();
    }

    // *********************************
    // ***** bezier edit mode  ****
    // *********************************

    glm::vec3 deltaX(_step,0,0);
    glm::vec3 deltaY(0,_step,0);
    glm::vec3 deltaZ(0,0,_step);

    if(action == GLFW_PRESS && keys[GLFW_KEY_B]) {
        _bezierEditModeEnabled = ! _bezierEditModeEnabled;

        _isInertiaEnabled = ! _isInertiaEnabled;
        // Reset inertia
        _isInerting = false;
        _lastDirection = glm::vec3(0.0, 0.0, 0.0);
        std::cout << "BEZIER EDIT MODE = " << _bezierEditModeEnabled << std::endl;
    }
    
    if (_bezierEditModeEnabled) {
        if (action == GLFW_PRESS && keys[GLFW_KEY_U]) {
             std::cout << "HANDLE 1" << std::endl;
            _selectedHandle = &_handle1;
        } else if (action == GLFW_PRESS && keys[GLFW_KEY_I]) {
             std::cout << "HANDLE 2" << std::endl;
            _selectedHandle = &_handle2;
        } else if (action == GLFW_PRESS && keys[GLFW_KEY_O]) {
            std::cout << "HANDLE 3" << std::endl;
            _selectedHandle = &_handle3;
        } else if (action == GLFW_PRESS && keys[GLFW_KEY_P]) {
            std::cout << "HANDLE 4" << std::endl;
            _selectedHandle = &_handle4;
        }


        if (action == GLFW_PRESS && keys[GLFW_KEY_X]) {
            _delta = deltaX;
            std::cout << "BEZIER EDIT SELECTED AXE = X " << std::endl;
        } else if (action == GLFW_PRESS && keys[GLFW_KEY_Z]) {
            _delta = deltaY;
            std::cout << "BEZIER EDIT SELECTED AXE = Y " << std::endl;
        } else if (action == GLFW_PRESS && keys[GLFW_KEY_Y]) {
            _delta = deltaZ;
            std::cout << "BEZIER EDIT SELECTED AXE = Z " << std::endl;
        }

        if (action == GLFW_PRESS  && keys[GLFW_KEY_UP]) {
            *_selectedHandle = *_selectedHandle - _delta;
            buildBezierCurve();
        } else if (action == GLFW_PRESS  && keys[GLFW_KEY_DOWN]) {
            *_selectedHandle = *_selectedHandle + _delta;
            buildBezierCurve();
        }

        if (action == GLFW_PRESS  && keys[GLFW_KEY_LEFT]) {
            _step += 0.2;
            std::cout << "BEZIER EDIT step = " << _step << std::endl;
        } else if (action == GLFW_PRESS  && keys[GLFW_KEY_RIGHT]) {
            _step -= 0.2;
            if (_step < 0) _step = 0;
            std::cout << "BEZIER EDIT step = " << _step << std::endl;
        }

        if (action == GLFW_PRESS && keys[GLFW_KEY_ENTER]) {
            std::cout << "camera/lookHulls.push_back(Hull(glm::"<< glm::to_string(_handle1) << ", glm::" << glm::to_string(_handle2) << ", glm::" << glm::to_string(_handle3) << ", glm::" << glm::to_string(_handle4) << "));"<<std::endl;
        }
    }
}

void MyWorld::updateFpsCameraPosition() {
    updateFlyCameraPosition();
    glm::vec3 cameraPosition = camera.getPosition();
    glm::vec2 pos_2d(cameraPosition.x, cameraPosition.z);
    //std::cout << "2D position: " << pos_2d.x << ", " << pos_2d.y << std::endl;
    glm::vec2 pos_texture(
        (pos_2d.x / Constants::TERRAIN_SCALE + 1.0) * 0.5,
        (pos_2d.y / Constants::TERRAIN_SCALE + 1.0) * 0.5
    );

    //std::cout << "Texture position: " << pos_texture.x << ", " << pos_texture.y << std::endl;
    float normalizedHeight = getHeight(pos_texture.x * FRAME_BUFFER_PERLIN_WIDTH, pos_texture.y * FRAME_BUFFER_PERLIN_HEIGHT);
    //std::cout << "Height: " << normalizedHeight << std::endl;
    float height = (normalizedHeight + 0.05) * Constants::TERRAIN_SCALE;
    if(keys[GLFW_KEY_SPACE] && !_hasJumped) {
        _hasJumped = true;
        _jumpStartTime = glfwGetTime();
        _jumpStartHeight = height;
    }

    if (_hasJumped) {
        GLfloat timeSinceJump = glfwGetTime() - _jumpStartTime;
        if (timeSinceJump <= 4.0) {
            GLfloat jumpHeight = 9 - pow(3 * timeSinceJump - 3, 2.0) + _jumpStartHeight;
            camera.setHeight(std::max(jumpHeight, height));
        } else {
            _hasJumped = false;
        }
    } else {
        camera.setHeight(height);
    }
}

float MyWorld::getHeight(unsigned int x, unsigned int y) {
    if (x < 512 && y < 512) {
        return _heightMap[x + 512 * y];
    } else {
        return 0.0;
    }
}
