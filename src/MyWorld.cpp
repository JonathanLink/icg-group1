#include "Constants.h"
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
    _bezierCurve.setScene(this);
    //_bezierCurve2.setScene(this);
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
    cameraHulls.clear();
    cameraHulls.push_back(Hull(_handle1, _handle2, _handle3, _handle4));
    //cameraHulls.push_back(Hull(glm::vec3(34.299995, 76.900009, -0.999998), glm::vec3(14.699999, 81.800011, -1.000000), glm::vec3(-16.400002, 80.399986, -5.100000), glm::vec3(-28.700003, 84.499985, -1.000000)));
    //cameraHulls.push_back(Hull(glm::vec3(0,23,-1), glm::vec3(0,20,-2), glm::vec3(0,20,-2), glm::vec3(0,20,-2)));
    //cameraHulls.push_back(Hull(glm::vec3(12,42,-1), glm::vec3(2,23,-5), glm::vec3(3,10,-3), glm::vec3(3,23,-2)));

    std::vector<Hull> lookHulls;
    lookHulls.clear();
    //lookHulls.push_back(Hull(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0)));
    lookHulls.push_back(Hull(glm::vec3(0, 80, 0), glm::vec3(0, 80, 0), glm::vec3(0, 80, 0), glm::vec3(0, 80, 0)));

    _cameraBezier.setHulls(cameraHulls, lookHulls);
    setCameraBezier(_cameraBezier);

    _bezierCurve.setPoints(_cameraBezier.getCameraCurvePoints());

    _bezierHandles.setHandles(cameraHulls, this);
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

    _bezierCurve.render(view, projection);
    _bezierHandles.render(view, projection);
    //_bezierCurve2.render(view, projection);

    _terrainReflectFB.cleanUp();
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

void MyWorld::keyCallback(int key, int /*scancode*/, int action, int /*mode*/) {
    
    // bezier edit mode logic
    glm::vec3 deltaX(_step,0,0);
    glm::vec3 deltaY(0,_step,0);
    glm::vec3 deltaZ(0,0,_step);

    if(action == GLFW_PRESS && keys[GLFW_KEY_B]) {
        _bezierEditModeEnabled = ! _bezierEditModeEnabled;
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
            std::cout << glm::to_string(_handle1) << ", " << glm::to_string(_handle2) << ", " << glm::to_string(_handle3) << ", " << glm::to_string(_handle4) << std::endl;
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
    if (x >= 0 && x < 512 && y >= 0 && y < 512) {
        return _heightMap[x + 512 * y];
    } else {
        return 0.0;
    }
}
