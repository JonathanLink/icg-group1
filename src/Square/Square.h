#ifndef SQUARE_H
#define SQUARE_H

#include "pgl/RenderObject.h"

class Square: public RenderObject {

public:
    Square();
    void init() override; 
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId;
    GLuint _elementBufferId;
    GLuint _texture1;
    GLuint _texture2;

};

#endif