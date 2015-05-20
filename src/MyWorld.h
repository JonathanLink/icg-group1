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

class MyWorld: public Scene {
public:
	MyWorld();
	void init() override;
	void render() override;
	void cleanUp() override;


private:
	void updateFpsCameraPosition() override;
	float getHeight(int x, int y);

	float* _heightMap;

	Perlin _perlin;
	Skybox _skybox;
	Terrain _terrain;
	FishEye _fishEye;
	Triangle _triangle;
	Square _square;
	Cube _cube;
	Water _water;
};

#endif // MYWORLD_H
