#ifndef HANDLE_H
#define HANDLE_H

#include "../../../Scene/RenderObject.h"

class Handle: public RenderObject {
public:
    Handle();
    void init() override; 
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;
    void setHandle(const glm::vec3 &handle);

private:
    GLuint _vertexArrayId;
    glm::vec3 _handle;
};

#endif