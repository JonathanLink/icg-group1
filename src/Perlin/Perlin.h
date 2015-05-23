#ifndef PERLIN_H
#define PERLIN_H

#include "../Scene/RenderObject.h"

class Perlin: public RenderObject {
public:
    Perlin();
    void init() override; 
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId;
};

#endif