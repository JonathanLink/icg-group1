#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"
#include "Scene.h"

class Window {

public:

    static const int OPENGL_VERSION_MAJOR = 3;
    static const int OPENGL_VERSION_MINOR = 3;
    static const int NUMBER_OF_SAMPLES_FOR_MULTISAMPLING = 4;

    static void init(GLuint width, GLuint height, const char* title);
    static void display(void (*display)(void));
    static void setScene(Scene* scene);
    static Scene* getScene();
    static void render();
    
    
};

#endif