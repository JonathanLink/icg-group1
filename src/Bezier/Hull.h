#ifndef HULL_H
#define HULL_H

#include <glm/glm.hpp>
#include <vector>

class Hull {
    public:
        Hull() {_p1 = _p2 = _p3 = _p4 = glm::vec3(0,0,0);}
        Hull(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4): _p1(p1), _p2(p2), _p3(p3), _p4(p4) { }
        glm::vec3& p1() { return _p1; }
        glm::vec3& p2() { return _p2; }
        glm::vec3& p3() { return _p3; }
        glm::vec3& p4() { return _p4; }
        std::vector<glm::vec3> getVector() { return std::vector<glm::vec3>{p1(), p2(), p3(), p4()}; }
    private:
        glm::vec3 _p1;
        glm::vec3 _p2;
        glm::vec3 _p3;
        glm::vec3 _p4;
};

#endif

