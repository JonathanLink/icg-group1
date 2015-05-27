#ifndef MYWORLD_CONSTANTS_H
#define MYWORLD_CONSTANTS_H

#include <GL/glew.h>
#include "glm/glm.hpp"

class Constants {
public:
    static float getParabola(float currentX, float width);

    static const float TERRAIN_SCALE;
    static const glm::vec3 SKYBOX_TRANSLATION;
    static const float SKYBOX_SCALE;
    static const bool IS_FULLSCREEN;
    static const GLuint WINDOW_WIDTH;
    static const GLuint WINDOW_HEIGHT;
    static const float WATER_HEIGHT;
};

#endif //MYWORLD_CONSTANTS_H
