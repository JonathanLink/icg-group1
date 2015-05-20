#include "MyWorld.h"
#include "pgl/FrameBuffer.h"

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
    const unsigned int FRAME_BUFFER_WIDTH = 512;
    const unsigned int FRAME_BUFFER_HEIGHT = 512;
    FrameBuffer perlinFrameBuffer = FrameBuffer(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
    GLuint perlinTextureId = perlinFrameBuffer.initTextureId(GL_R32F); 
    perlinFrameBuffer.bind();
        _perlin.render(view, projection);
    perlinFrameBuffer.unbind();

    _heightMap = new float[512 * 512];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, perlinTextureId);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, _heightMap);

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

    _skybox.render(view, projection);
    _terrain.render(view, projection);
    _water.render(view, projection);
}

void MyWorld::cleanUp() {
    _skybox.cleanUp();
    _terrain.cleanUp();
    _perlin.cleanUp();
    _fishEye.cleanUp();
}

void MyWorld::updateFpsCameraPosition() {

}

float MyWorld::getHeight(int x, int y) {
    return _heightMap[x + 512 * y];
}