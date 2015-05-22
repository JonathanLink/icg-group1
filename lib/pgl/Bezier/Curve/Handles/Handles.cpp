

#include "pgl/Bezier/Curve/Handles/Handles.h"

Handles::Handles() {
    // Do nothing
}

void Handles::setHandles(const std::vector<Hull> &cameraHulls, Scene *scene) {
    clear();
    
    for (int i = 0; i < cameraHulls.size(); ++i) {
        Hull currentHull = cameraHulls[i];
        std::vector<glm::vec3> hulls = currentHull.getVector();
        for (int j = 0; j < hulls.size(); ++j) {
            Handle handle;
            handle.setScene(scene);
            glm::vec3 handlePos = hulls[j];
            handle.setHandle(handlePos);
            _handles.push_back(handle);
        }
    }
   
}

void Handles::render(const glm::mat4 &view, const glm::mat4 &projection) {
    for (int i = 0; i <_handles.size(); ++i) {
        Handle handle = _handles[i];
        handle.render(view, projection);
    }
}

void Handles::cleanUp() {
    for (int i = 0; i <_handles.size(); ++i) {
        Handle handle = _handles[i];
        handle.cleanUp();
    }
}

void Handles::clear() {
    _handles.clear();
}