#ifndef BEZIER_H
#define BEZIER_H
#include <iostream>
#include <vector>
#include "Hull.h"

class Bezier {
public:
    Bezier();
    void addHulls(std::vector<Hull> hulls);
    glm::vec3 samplePointAtTime(double time);
    std::vector<glm::vec3> getVertices(); 
    void clear();

private:
    void bezier(Hull &hull, int depth=0);
    void computeParameterization();
    double distance(const glm::vec3 p1, const glm::vec3 p2);
    
    double _totalDistance;
    std::vector<glm::vec3> _vertices;
    std::vector<double> _param;
};

#endif