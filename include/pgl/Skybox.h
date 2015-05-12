#ifndef SKYBOX_H
#define SKYBOX_H

#include "common.h"
#include "RenderObject.h"

class Skybox: public RenderObject {
public:
    Skybox();
    void init() override;
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;

private:
    GLuint loadCubemap(std::vector<const GLchar*> faces);
    GLuint _cubemapTextureId;
    GLuint _VAO;
};

#endif