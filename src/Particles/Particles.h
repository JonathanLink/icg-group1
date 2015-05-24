#ifndef MYWORLD_PARTICLES_H
#define MYWORLD_PARTICLES_H

#include "../Scene/RenderObject.h"

class Particles: public RenderObject {
public:
    Particles();
    void init() override;
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId;
    std::vector<GLfloat> _vertices;

    GLfloat _initialTime;
    float randomOffset();
    float randomNumber(float minimu, float maximum);
    float MAX_HEIGHT = 100.0f;
    float DESCENT_TIME = 2.0f;
    std::vector<glm::vec3> _particlePositions;
};


#endif //MYWORLD_PARTICLES_H
