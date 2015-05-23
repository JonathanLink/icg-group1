#ifndef CURVE_H
#define CURVE_H

#include "../../RenderObject.h"

class Curve: public RenderObject {
public:
    Curve();
    void init() override; 
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;
    void setPoints(std::vector<glm::vec3> points);

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId; 
    std::vector<glm::vec3> _points;
};

#endif