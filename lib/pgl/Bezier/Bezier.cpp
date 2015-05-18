#include "pgl/Bezier/Bezier.h"

Bezier::Bezier() {
    // Do nothing
}


void Bezier::init() {
    std::cout << "Init Bezier" << std::endl;

    loadShaders( "../lib/pgl/Bezier/bezier_vshader.glsl", "../lib/pgl/Bezier/bezier_fshader.glsl" );

    
}

void Bezier::bezier(Hull &hull, int depth) {
    Hull l;
    Hull r;

    //--- endpoints
    l.p1() = hull.p1();
    r.p4() = hull.p4();

    //--- one-in
    l.p2() = (hull.p1() + hull.p2()) / 2.0;
    r.p3() = (hull.p3() + hull.p4()) / 2.0;

    //--- interior
    l.p3() = ( l.p2() + (hull.p2() + hull.p3())/2.0 )/2.0;
    r.p2() = ( r.p3() + (hull.p2() + hull.p3())/2.0 )/2.0;

    //--- middle
    l.p4() = ( l.p3() + r.p2() ) / 2.0;
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


void Bezier::addHulls(const std::vector<Hull>& hulls) {
    _hulls  = hulls;
    for (std::size_t i = 0; i != _hulls.size(); ++i) {
        bezier(_hulls[i]);
    }
    computeParameterization();
}

void Bezier::computeParameterization() {
    double prevDist = 0.0;
    for (int i = 1; i < _vertices.size(); ++i) {
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


void Bezier::render(const glm::mat4 &view, const glm::mat4 &projection) {


}

void Bezier::cleanUp() {
    std::cout << "Cleanup Bezier" << std::endl;
}




