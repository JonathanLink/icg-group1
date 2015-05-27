#ifndef MYWORLD_H
#define MYWORLD_H

#include "Scene.h"
#include "../Skybox/Skybox.h"
#include "../Bezier/BezierController.h"
#include "../Terrain/Terrain.h"
#include "../Perlin/Perlin.h"
#include "../Cube/Cube.h"
#include "../Water/Water.h"
#include "../FrameBuffer/FrameBuffer.h"
#include "../Particles/Particles.h"
#include "../Sun/Sun.h"


class MyWorld: public Scene {
public:


	MyWorld(unsigned int terrainReflectFbWidth,
			unsigned int terrainReflectFbHeight);
	void init() override;
	void render() override;
	void cleanUp() override;
	void keyCallback(int key, int scancode, int action, int mode) override;

private:
	void updateFpsCameraPosition() override;
	void buildBezierCurve();
	float getHeight(unsigned int x, unsigned int y);
	void generateSkyViewCurve();
	void generateLakeCurve();
	void generateAroundCurve();
	void drawPerlin();
	GLuint _perlinTextureId;

	float* _heightMap;

	Perlin _perlin;
	Skybox _skybox;
	Terrain _terrain;

	Water _water;
	Particles _particles;
	bool _wireframeIsEnabled;
	bool _particlesEnabled = false;

	BezierController _bezierController;

	Sun _sun;

	FrameBuffer _terrainReflectFB;
};

#endif // MYWORLD_H
