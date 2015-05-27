#ifndef PERLIN_H
#define PERLIN_H

#include "../Scene/RenderObject.h"

class Perlin: public RenderObject {
public:


	enum ParamNoise {
        LACUNARITY,
        FREQ,
        OCTAVES,
        AMPLITUDE,
        X,
        FRAMEBUFFER
    };

    Perlin();
    void init() override; 
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;
    void keyCallback(int key, int scancode, int action, int mode);
    bool isPerlinModeIsEnabled();
    int getFrameBufferWidth();

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId;

    int _frameBufferWidth;
    ParamNoise _param;
    bool _perlinModeIsEnabled = false;
    float _lacunarity;
	float _freq;
	int _octaves; 
	float _amplitude;
    float _x; 
};

#endif