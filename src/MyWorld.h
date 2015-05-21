#ifndef MYWORLD_H
#define MYWORLD_H

#include "pgl/Scene.h"
#include "pgl/Skybox/Skybox.h"
#include "pgl/Bezier/Curve/Curve.h"
#include "pgl/Bezier/Curve/Handles/Handles.h"
#include "Terrain/Terrain.h"
#include "Perlin/Perlin.h"
#include "FishEye/FishEye.h"
#include "Cube/Cube.h"
#include "Water/Water.h"


class MyWorld: public Scene {
public:

	static const unsigned int FRAME_BUFFER_PERLIN_WIDTH = 512;
    static const unsigned int FRAME_BUFFER_PERLIN_HEIGHT = 512;

	MyWorld();
	void init() override;
	void render() override;
	void cleanUp() override;
	void keyCallback(int key, int scancode, int action, int mode) override;


private:
	void updateFpsCameraPosition() override;
	float getHeight(int x, int y);
	void buildBezierCurve();

	float* _heightMap;

	Perlin _perlin;
	Skybox _skybox;
	Terrain _terrain;
	FishEye _fishEye;
	Cube _cube;
	Water _water;

	// Bezier 
	bool _bezierEditModeEnabled;
	CameraBezier _cameraBezier;
	Curve _bezierCurve;
	Curve _bezierCurve2;
	Handles _bezierHandles;
	glm::vec3 *_selectedHandle;
	glm::vec3 _handle1;
	glm::vec3 _handle2;
	glm::vec3 _handle3;
	glm::vec3 _handle4;
	

};

#endif // MYWORLD_H
