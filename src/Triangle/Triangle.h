#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "pgl/RenderObject.h"

class Triangle: public RenderObject {
public:
    Triangle();
    void init() override;
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId;
};

#endif