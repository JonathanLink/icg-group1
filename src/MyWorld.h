#ifndef MYWORLD_H
#define MYWORLD_H

#include "pgl/Scene.h"
#include "Terrain/Terrain.h"
#include "Perlin/Perlin.h"
#include "Triangle/Triangle.h"
#include "Square/Square.h"
#include "Cube/Cube.h"

class MyWorld: public Scene {
public:
	MyWorld();
	void init() override;
	void render() override;
	void cleanUp() override;

private:
	Perlin _perlin;
	Terrain _terrain;
	Triangle _triangle;
	Square _square;
	Cube _cube;
};

#endif // MYWORLD_H
