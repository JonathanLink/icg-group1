#ifndef CURVE_H
#define CURVE_H

#include "../../Scene/RenderObject.h"

class Curve: public RenderObject {
public:
    Curve();
    void init() override; 
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;
    void setPoints(std::vector<glm::vec3> points);
    void setIsLookAtCurve(bool isLookAtCurve);

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId; 
    std::vector<glm::vec3> _points;
    bool _isLookAtCurve;
};

#endif