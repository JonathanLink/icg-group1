#ifndef MYWORLD_H
#define MYWORLD_H

#include "pgl/Scene.h"
#include "pgl/Skybox/Skybox.h"
#include "Terrain/Terrain.h"
#include "Perlin/Perlin.h"
#include "FishEye/FishEye.h"

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

};

#endif // MYWORLD_H
