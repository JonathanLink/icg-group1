#ifndef WINDOW_H
#define WINDOW_H

#include "../Scene/Scene.h"

class Window {
public:
    static Window& getInstance();

    const int OPENGL_VERSION_MAJOR = 3;
    const int OPENGL_VERSION_MINOR = 3;
    const int NUMBER_OF_SAMPLES_FOR_MULTISAMPLING = 4;

    void setScene(Scene* scene);
    Scene* getScene();
    void render();

private:
    Window(GLuint width, GLuint height, const char* title);
    Window(Window const&);
    void operator=(Window const&);

    static void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void window_mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    GLuint _windowWidth;
    GLuint _windowHeight;
    const char* _title;
    GLFWwindow* _window;
    Scene* _scene;

};

#endif