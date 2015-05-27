#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "../libs/stb_image.h"
#include "Scene.h"


class RenderObject {
public:
    RenderObject();
    virtual void init() =0;
    virtual void render(const glm::mat4 &view, const glm::mat4 &projection) =0;
    virtual void cleanUp() =0;

    void setScene(Scene *scene);
    void loadShaders(const char* vertexShaderFile,const char* fragmentShaderFile);
    void useShaders();
    GLuint gen2DTexture(const char* imagePath, GLint format);
    void keyCallback(int key, int scancode, int action, int mode); 
    void mouseCallback(double xpos, double ypos);

protected:
    Scene *scene;
    GLuint pid;
    glm::mat4 model;

};

#endif