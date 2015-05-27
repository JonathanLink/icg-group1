
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "Constants.h"

const float Constants::TERRAIN_SCALE = 35.0f;
const glm::vec3 Constants::SKYBOX_TRANSLATION = glm::vec3(0.0f, 30.0f, 0.0f);
const float Constants::SKYBOX_SCALE = 55.0f;
const bool Constants::IS_FULLSCREEN = false;
const GLuint Constants::WINDOW_WIDTH = 800;
const GLuint Constants::WINDOW_HEIGHT = 600;

float Constants::getParabola(float t, float duration) {
   return - pow(t - duration / 2.0f, 2.0) + pow(duration / 2.0f, 2.0);
}
