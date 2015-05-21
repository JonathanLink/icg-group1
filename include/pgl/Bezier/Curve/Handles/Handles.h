#ifndef HANDLES_H
#define HANDLES_H

#include "../../Hull.h"
#include "Handle.h"

class Handles {

public:
    Handles();
    void setHandles(const std::vector<Hull> &cameraHulls, Scene *scene);
    void render(const glm::mat4 &view, const glm::mat4 &projection);
    void cleanUp();
    void clear();

private:
	std::vector<Handle> _handles;
 
};

#endif