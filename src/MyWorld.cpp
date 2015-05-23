#include "MyWorld.h"
#include "glm/gtc/matrix_transform.hpp"

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
    const unsigned int FRAME_BUFFER_WIDTH = 512;
    const unsigned int FRAME_BUFFER_HEIGHT = 512;
    FrameBuffer perlinFrameBuffer = FrameBuffer(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
    GLuint perlinTextureId = perlinFrameBuffer.initTextureId(GL_R32F); 
    perlinFrameBuffer.bind();
        _perlin.render(view, projection);
    perlinFrameBuffer.unbind();

    // Save height map in memory
    _heightMap = new float[512 * 512];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, perlinTextureId);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, _heightMap);

    _water.setTexturePerlin(perlinTextureId);
    _terrain.setTexture(perlinTextureId);

    // set camera bezier
    std::vector<Hull> cameraHulls;
    cameraHulls.push_back(Hull(glm::vec3(0,23,-1), glm::vec3(0,20,-2), glm::vec3(0,20,-2), glm::vec3(0,20,-2)));

    std::vector<Hull> lookHulls;
    lookHulls.push_back(Hull(glm::vec3(4,22,-1), glm::vec3(0,21,-2), glm::vec3(-5,24,-2), glm::vec3(-9,10,2)));

    CameraBezier cameraBezier;
    cameraBezier.setHulls(cameraHulls, lookHulls);
    setCameraBezier(cameraBezier);
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
}

void MyWorld::cleanUp() {
    _skybox.cleanUp();
    _terrain.cleanUp();
    _perlin.cleanUp();
    _fishEye.cleanUp();
}

void MyWorld::updateFpsCameraPosition() {
    updateFlyCameraPosition();
    glm::vec3 cameraPosition = camera.getPosition();
    glm::vec2 pos_2d(cameraPosition.x, cameraPosition.z);
    std::cout << "2D position: " << pos_2d.x << ", " << pos_2d.y << std::endl;
    glm::vec2 pos_texture(
        (pos_2d.x / 35.0 + 1.0) * 0.5,
        (pos_2d.y / 35.0 + 1.0) * 0.5
    );

    //std::cout << "Texture position: " << pos_texture.x << ", " << pos_texture.y << std::endl;
    float normalizedHeight = getHeight(pos_texture.x * 512.0, pos_texture.y * 512.0);
    //std::cout << "Height: " << normalizedHeight << std::endl;
    float height = (normalizedHeight + 0.05) * 35.0;
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
