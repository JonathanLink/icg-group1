#include "MyWorld.h"
#include "pgl/FrameBuffer.h"

MyWorld::MyWorld()  {

}

void MyWorld::init() {

	_perlin.setScene(this);
	_terrain.setScene(this);
	
	_square.setScene(this);
    _cube.setScene(this);

	// Draw perlin noise in framebuffer we've just created
	FrameBuffer frameBuffer = FrameBuffer(1024, 1024);
	GLuint textureId = frameBuffer.initTextureId(); 
	frameBuffer.bind();
		_perlin.render(view, projection);
	frameBuffer.unbind();

	_terrain.setTexture(textureId);
	

}

void MyWorld::render() {

	bool renderTerrain = true;
	if (renderTerrain) {
		_terrain.render(view, projection);
	} else {
		_perlin.render(view, projection);
	}
	

	// uncomment this line to have a surprise 
    //_cube.render(view, projection);
}

void MyWorld::cleanUp() {
	_terrain.cleanUp();
    _square.cleanUp();
    _cube.cleanUp();
}