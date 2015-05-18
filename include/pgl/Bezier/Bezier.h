#ifndef BEZIER_H
#define BEZIER_H

#include "../common.h"
#include "../RenderObject.h"
#include "Hull.h"


class Bezier: public RenderObject {
public:
    
    Bezier();
    void init() override;
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;
    
    void addHulls(const std::vector<Hull>& hulls);
    glm::vec3 samplePointAtTime(double time);


private:

    void bezier(Hull &hull, int depth=0);
    void computeParameterization();
    double distance(const glm::vec3 p1, const glm::vec3 p2);

    GLuint _VAO;
    GLuint _VBO;
    double _totalDistance;
    std::vector<Hull> _hulls;
    std::vector<glm::vec3> _vertices;
    std::vector<double> _param;


};

#endif