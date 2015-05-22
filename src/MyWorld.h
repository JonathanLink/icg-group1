#ifndef MYWORLD_H
#define MYWORLD_H

#include "pgl/Scene.h"
#include "pgl/Skybox/Skybox.h"
#include "Terrain/Terrain.h"
#include "Perlin/Perlin.h"
#include "FishEye/FishEye.h"
#include "Triangle/Triangle.h"
#include "Square/Square.h"
#include "Cube/Cube.h"
#include "Water/Water.h"
#include "pgl/FrameBuffer.h"

class MyWorld: public Scene {
public:
	MyWorld();
	void init() override;
	void render() override;
	void cleanUp() override;

private:
	Perlin _perlin;
	Skybox _skybox;
	Terrain _terrain;
	FishEye _fishEye;
	Triangle _triangle;
	Square _square;
	Cube _cube;
	Water _water;
	FrameBuffer _terrainReflectFB;


};

#endif // MYWORLD_H
