#include "../Constants/Constants.h"
#include "MyWorld.h"
#include "../Window/Window.h"

MyWorld::MyWorld(unsigned int terrainReflectFbWidth,
                 unsigned int terrainReflectFbHeight):
        Scene(glm::vec3(-0.967917f, 20.54413f, -1.45086f),
              glm::vec3(-22.4157f, 36.1665f, 0.0f)),
        _terrainReflectFB(terrainReflectFbWidth, terrainReflectFbHeight) {
    // Do nothing
}

void MyWorld::init() {
    _skybox.setScene(this);
    _perlin.setScene(this);
    _terrain.setScene(this);
    _water.setScene(this);
    _particles.setScene(this);

    drawPerlin();

    // Bezier init
    _step = 0.5;
    _bezierEditModeEnabled = false;
    _bezierPositionCurve.setScene(this);
    _bezierLookCurve.setScene(this);

    // CURVE OK
    _handle1 = glm::vec3(-33.299969, 29.899982, 19.099995);
    _handle2 = glm::vec3(-23.700001, 11.299978, 1.400000);
    _handle3 = glm::vec3(-24.099998, 6.599979, -7.899996);
    _handle4 = glm::vec3(13.699999, 16.199974, -3.599999);
    _selectedHandle = &_handle1;
    buildBezierCurve();

    _wireframeIsEnabled = false;
}

void MyWorld::generateSkyViewCurve() {
    _handle1 = glm::vec3(0.0, 65.0, 20.0);
    _handle2 = glm::vec3(0.0, 65.0, 1.0);
    _handle3 = glm::vec3(0.0,65.0, -1.0);
    _handle4 = glm::vec3(0.0, 65.0, -20.0);

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
    std::vector<Hull> cameraHulls;
    cameraHulls.clear();
    cameraHulls.push_back(Hull(_handle1, _handle2, _handle3, _handle4));
    //cameraHulls.push_back(Hull(glm::vec3(-20.799969, 17.399982, 19.099995), glm::vec3(-31.500004, 10.299983, 3.400000), glm::vec3(-4.599994, 10.499980, -7.899996), glm::vec3(13.699999, 16.199974, -3.599999)));

    std::vector<Hull> lookHulls;
    lookHulls.clear();
    lookHulls.push_back(Hull(glm::vec3(-25.799969, 20.399982, 25.099995), glm::vec3(-32.500004, 13.299983, 4.400000), glm::vec3(-15.599994, 12.499980, 5.4f), glm::vec3(-12.699999, 15.199974, 8.599999)));

    _cameraBezierLake.setHulls(cameraHulls, lookHulls);
    setCameraBezier(_cameraBezierLake);

    _bezierPositionCurve.setPoints(_cameraBezierLake.getCameraCurvePoints());
    _bezierLookCurve.setIsLookAtCurve(true);
    _bezierLookCurve.setPoints(_cameraBezierLake.getLookCurvePoints());

    _bezierHandles.setHandles(cameraHulls, this);
}


void MyWorld::generateAroundCurve() {
    _handle1 = glm::vec3(0, 30, 35);
    _handle2 = glm::vec3(-10, 120, 1);
    _handle3 = glm::vec3(-20, 75, -1);
    _handle4 = glm::vec3(-30, 20, -15);

    std::vector<Hull> cameraHulls;
    cameraHulls.clear();
    //cameraHulls.push_back(Hull(glm::vec3(11.000000, 73.200001, 20.000000), glm::vec3(-127.099945, 72.799999, 0.200014), glm::vec3(16.299997, 75.799995, -93.000000), glm::vec3(32.200005, 30.000000, 28.500000)));
    cameraHulls.push_back(Hull(glm::vec3(0, 30, 35), glm::vec3(-10, 120, 1), glm::vec3(-20, 75, -1), glm::vec3(-30, 20, -15)));
    //cameraHulls.push_back(Hull(_handle1, _handle2, _handle3, _handle4));

    std::vector<Hull> lookHulls;
    lookHulls.clear();
    //lookHulls.push_back(Hull(glm::vec3(0.299997, 27.999977, -10.099998), glm::vec3(2.600057, 14.799999, 8.100016), glm::vec3(4.599998, 30.799995, 2.000002), glm::vec3(3.500003, 25.500000, 8.299999)));
    //lookHulls.push_back(Hull(glm::vec3(2.799998, 42.999977, 15.400003), glm::vec3(-22.599945, 26.799999, 23.700014), glm::vec3(-6.200003, 45.799995, -67.000000), glm::vec3(22.700005, 31.500000, 21.500000)));
    lookHulls.push_back(Hull(glm::vec3(-20, 0, -20), glm::vec3(20, 0, 0), glm::vec3(20, 0, 0), glm::vec3(-20, 0, 20)));
    

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
    //generateAroundCurve();
}


void MyWorld::drawPerlin() {
    // Draw perlin noise in framebuffer we've just created
    FrameBuffer perlinFrameBuffer = FrameBuffer(FRAME_BUFFER_PERLIN_WIDTH, FRAME_BUFFER_PERLIN_HEIGHT);
    _perlinTextureId = perlinFrameBuffer.initTextureId(GL_R32F); 
    perlinFrameBuffer.bind();
        _perlin.render(view, projection);
    perlinFrameBuffer.unbind();

    // Save height map in memory
    _heightMap = new float[FRAME_BUFFER_PERLIN_WIDTH * FRAME_BUFFER_PERLIN_HEIGHT];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _perlinTextureId);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, _heightMap);

    _water.setTexturePerlin(_perlinTextureId);
    _terrain.setTexture(_perlinTextureId);
}

void MyWorld::render() {

    
    //Draw terrain in framebuffer for water reflection
    if (true) { // to debug perlin purpose

        if (_perlin.isPerlinModeIsEnabled()) {
            drawPerlin();
        }
        

        GLuint terrainReflectTextureId = _terrainReflectFB.initTextureId(GL_RGB);
        _terrainReflectFB.bind();
            _terrain.setReflection(true);
            _terrain.render(view, projection);
            _terrain.setReflection(false);
        _terrainReflectFB.unbind();
        _water.setTextureMirror(terrainReflectTextureId);

        if (_wireframeIsEnabled) {
             glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // wireframe 
         } else {
             glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); 
         }
        _skybox.render(view, projection);
        _terrain.render(view, projection);
        _water.render(view, projection);

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

         _terrainReflectFB.cleanUp();
        _particles.render(view, projection);

        if (_bezierEditModeEnabled) {
            _bezierPositionCurve.render(view, projection);
            _bezierLookCurve.render(view, projection);
            _bezierHandles.render(view, projection);
        }
    } else {
        _perlin.render(view, projection);
    }
    
}

void MyWorld::cleanUp() {
    _skybox.cleanUp();
    _terrain.cleanUp();
    _perlin.cleanUp();

    _bezierPositionCurve.cleanUp();
    _bezierLookCurve.cleanUp();
    _bezierHandles.cleanUp();
    _particles.cleanUp();
}

// This method is ugly and i know it!
void MyWorld::keyCallback(int key, int scancode, int action, int mode) {
    

   if (!_bezierEditModeEnabled) {
        _perlin.keyCallback(key, scancode, action, mode);
   }

   if (action == GLFW_PRESS && key == GLFW_KEY_L) {
        _wireframeIsEnabled = !_wireframeIsEnabled;
   }

    // *********************************
    // ****bezier ajust speed ****
    // *********************************

    if (action == GLFW_PRESS && key == GLFW_KEY_K) {
        _cameraBezier.increasePeriod();
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_L) {
        _cameraBezier.decreasePeriod();
    }

    // *********************************
    // ***** bezier edit mode  ****
    // *********************************

    glm::vec3 deltaX(_step,0,0);
    glm::vec3 deltaY(0,_step,0);
    glm::vec3 deltaZ(0,0,_step);

    if(action == GLFW_PRESS && key == GLFW_KEY_B) {
        _bezierEditModeEnabled = ! _bezierEditModeEnabled;

        _isInertiaEnabled = ! _isInertiaEnabled;
        // Reset inertia
        _isInerting = false;
        _lastDirection = glm::vec3(0.0, 0.0, 0.0);
        std::cout << "BEZIER EDIT MODE = " << _bezierEditModeEnabled << std::endl;
    }
    
    if (_bezierEditModeEnabled) {
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

        if (action == GLFW_PRESS  && key == GLFW_KEY_LEFT) {
            _step += 0.2;
            std::cout << "BEZIER EDIT step = " << _step << std::endl;
        } else if (action == GLFW_PRESS  && key == GLFW_KEY_RIGHT) {
            _step -= 0.2;
            if (_step < 0) _step = 0;
            std::cout << "BEZIER EDIT step = " << _step << std::endl;
        }

        if (action == GLFW_PRESS && key == GLFW_KEY_ENTER) {
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
        const static float MAX_JUMP_TIME = 3.0f;
        if (timeSinceJump <= MAX_JUMP_TIME) {
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
    if (x < FRAME_BUFFER_PERLIN_WIDTH && y < FRAME_BUFFER_PERLIN_HEIGHT) {
        return _heightMap[x + FRAME_BUFFER_PERLIN_WIDTH * y];
    } else {
        return 0.0;
    }
}
