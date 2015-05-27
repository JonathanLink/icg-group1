#ifndef MYWORLD_SUN_H
#define MYWORLD_SUN_H


#include "../Scene/RenderObject.h"

class Sun: public RenderObject {
public:
    Sun();
    void init() override;
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;

    void setPosition(const glm::vec3 position);
private:
    GLuint _vertexArrayId;
    glm::vec3 _position;
};


#endif //MYWORLD_SUN_H
