#ifndef MYWORLD_CONSTANTS_H
#define MYWORLD_CONSTANTS_H

#include "glm/glm.hpp"

class Constants {
public:
    static const float TERRAIN_SCALE;
    static const glm::vec3 SKYBOX_TRANSLATION;
    static const float SKYBOX_SCALE;

//    static constexpr glm::vec3 getSkyboxTranslate() {
//        return glm::vec3(0.0f, 30.0f, 0.0f);
//    }
//    static constexpr float getSkyboxScale() {
//        return 55.0f;
//    }
};

#endif //MYWORLD_CONSTANTS_H
