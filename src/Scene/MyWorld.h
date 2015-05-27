#ifndef MYWORLD_H
#define MYWORLD_H

#include "Scene.h"
#include "../Skybox/Skybox.h"
#include "../Bezier/Curve/Curve.h"
#include "../Bezier/Curve/Handles/Handles.h"
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

	// Bezier 
	glm::vec3 _delta;
	float _step;
	bool _bezierEditModeEnabled;
	CameraBezier _cameraBezierTopView;
	CameraBezier _cameraBezierLake;
	Curve _bezierPositionCurve;
	Curve _bezierLookCurve;
	Handles _bezierHandles;
	glm::vec3 *_selectedHandle;
	glm::vec3 _handle1;
	glm::vec3 _handle2;
	glm::vec3 _handle3;
	glm::vec3 _handle4;

	Sun _sun;

	FrameBuffer _terrainReflectFB;
};

#endif // MYWORLD_H
