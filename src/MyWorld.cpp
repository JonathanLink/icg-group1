#include "MyWorld.h"
#include "pgl/FrameBuffer.h"

MyWorld::MyWorld() {
    // Do nothing
}

void MyWorld::init() {
	
	_skybox.setScene(this);
	_perlin.setScene(this);
	_terrain.setScene(this);


	// Draw perlin noise in framebuffer we've just created
    FrameBuffer frameBuffer = FrameBuffer(FRAMEBUFFER_WIDTH,
                                          FRAMEBUFFER_WIDTH);
	GLuint textureId = frameBuffer.initTextureId(); 
	frameBuffer.bind();
		_perlin.render(view, projection);
	frameBuffer.unbind();

	_terrain.setTexture(textureId);
}

void MyWorld::render() {
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // wireframe
	_skybox.render(view, projection);
	_terrain.render(view, projection); 
}

void MyWorld::cleanUp() {
	_skybox.cleanUp();
	_terrain.cleanUp();
}
