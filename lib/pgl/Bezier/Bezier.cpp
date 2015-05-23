#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"

#include "pgl/Bezier/Hull.h"
#include "pgl/Bezier/Bezier.h"

Bezier::Bezier() {
    // Do nothing
}



void Bezier::bezier(Hull &hull, int depth) {
    Hull l;
    Hull r;

    if (depth == 0) { std::cout << ">: " << hull.p1().x << " depth=" << depth << std::endl; }

    //--- endpoints
    l.p1() = hull.p1();
    r.p4() = hull.p4();

    //--- one-in
    l.p2() = (hull.p1() + hull.p2()) / 2.0f;
    r.p3() = (hull.p3() + hull.p4()) / 2.0f;

    //--- interior
    l.p3() = ( l.p2() + (hull.p2() + hull.p3())/2.0f )/2.0f;
    r.p2() = ( r.p3() + (hull.p2() + hull.p3())/2.0f )/2.0f;

    //--- middle
    l.p4() = ( l.p3() + r.p2() ) / 2.0f;
    r.p1() = l.p4();

    //--- recursion v.s. draw
    if (depth < 5) {
        bezier(l,depth+1);
        bezier(r,depth+1);
    } else {
        _vertices.push_back(l.p1());
    }

    _vertices.push_back(hull.p4());
}


void Bezier::addHulls(std::vector<Hull> hulls) {
    clear();
    for (int i = 0; i < hulls.size(); ++i) {
        bezier(hulls[i]);
    }
    computeParameterization();

}

void Bezier::computeParameterization() {
    double prevDist = 0.0;
    for (int  i = 1; i < _vertices.size(); ++i) {
        glm::vec3 vertice = _vertices.at(i);
        glm::vec3 prevPoint = _vertices.at(i - 1);
        double  dist = prevDist + distance(prevPoint, vertice);
        _param.push_back(dist);
        prevDist = dist;
    }
}

double Bezier::distance(const glm::vec3 p1, const glm::vec3 p2) {
    double diffX = p1.x - p2.x;
    double diffY = p1.y - p2.y;
    double diffZ = p1.z - p2.z;
    return sqrt( ( diffX * diffX ) + ( diffY * diffY ) + ( diffZ * diffZ) );
}

glm::vec3 Bezier::samplePointAtTime(double time) {
    if ( !_vertices.empty() && 
        !_param.empty() && 
        time > 0.0 &&
        time <= 1.0) {
        int maxParamIndex = _param.size() - 1;
        float curve_length = _param.at(maxParamIndex);
        float distance = time * curve_length;

        int i = 0;
        while(i <= maxParamIndex && _param.at(i) < distance) {
            i = i + 1;
        }

        // linear interpolation
        float mu = (distance - _param[i-1]) / (_param[i] - _param[i-1]);
        return mu * _vertices[i+1] + (1 - mu) * _vertices[i];
    }
    return glm::vec3();
}

std::vector<glm::vec3> Bezier::getVertices() {
    return _vertices;
}

void Bezier::clear() {
    _vertices.clear();
    _param.clear();
}



