#include "MyWorld.h"
#include "pgl/FrameBuffer.h"

MyWorld::MyWorld() {
    // Do nothing
}

void MyWorld::init() {
    _skybox.setScene(this);
    _perlin.setScene(this);
    _terrain.setScene(this);
    _water.setScene(this);
    //_fishEye.setScene(this);


    // Draw perlin noise in framebuffer we've just created
    FrameBuffer perlinFrameBuffer = FrameBuffer(512, 512);
    GLuint perlinTextureId = perlinFrameBuffer.initTextureId(GL_R32F); 
    perlinFrameBuffer.bind();
        _perlin.render(view, projection);
    perlinFrameBuffer.unbind();

    _terrain.setTexture(perlinTextureId);
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
