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
    std::vector<GLuint> _indices;
};


#endif //MYWORLD_PARTICLES_H
