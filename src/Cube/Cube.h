#ifndef CUBE_H
#define CUBE_H

#include "pgl/RenderObject.h"

class Cube: public RenderObject {

public:
    Cube();
    void init() override; 
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId;
    GLuint _elementBufferId;
    GLuint _texture1;

};

#endif