#ifndef FISH_EYE_H
#define FISH_EYE_H

#include "../RenderObject.h"

class FishEye: public RenderObject {
public:
    FishEye();
    void init() override; 
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;
    void setTexture(GLuint textureId);

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId;
    GLuint _textureId;
};

#endif