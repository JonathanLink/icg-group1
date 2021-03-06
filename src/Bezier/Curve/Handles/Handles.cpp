

#include "Handles.h"

Handles::Handles() {
    // Do nothing
}

void Handles::setHandles(const std::vector<Hull> &cameraHulls, Scene *scene) {
    clear();
    
    for (unsigned int i = 0; i < cameraHulls.size(); ++i) {
        Hull currentHull = cameraHulls[i];
        std::vector<glm::vec3> hulls = currentHull.getVector();
        unsigned int j =  (i > 0) ? 1 : 0;
        for (;j < hulls.size(); ++j) {
            Handle handle;
            handle.setScene(scene);
            glm::vec3 handlePos = hulls[j];
            handle.setHandle(handlePos);
            _handles.push_back(handle);
        }
    }
   
}

void Handles::render(const glm::mat4 &view, const glm::mat4 &projection) {
    for (unsigned int i = 0; i <_handles.size(); ++i) {
        Handle handle = _handles[i];
        handle.render(view, projection);
    }
}

void Handles::cleanUp() {
    for (unsigned int i = 0; i <_handles.size(); ++i) {
        Handle handle = _handles[i];
        handle.cleanUp();
    }
}

void Handles::clear() {
    _handles.clear();
}