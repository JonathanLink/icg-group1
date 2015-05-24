#include <iostream>
#include "Window.h"

Window::Window(GLuint width, GLuint height, const char* title) {
    _title = title;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    // for MacOSX compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // for antialiasing
    glfwWindowHint(GLFW_SAMPLES, NUMBER_OF_SAMPLES_FOR_MULTISAMPLING);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  

    _window = glfwCreateWindow(width, height, _title, nullptr, nullptr);
    _windowWidth = width;
    _windowHeight = height;
//    int actualWidth, actualHeight;
//    glfwGetFramebufferSize(_window, &actualWidth, &actualHeight);
//    fprintf(stderr, "Actual: (%d, %d)\n", actualWidth, actualHeight);
//    fprintf(stderr, "Given: (%d, %d)\n", width, height);
//    _windowWidth = actualWidth;
//    _windowHeight = actualHeight;

    glfwMakeContextCurrent(_window);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    // set  callback functions
    glfwSetKeyCallback(_window, Window::window_key_callback);
    glfwSetCursorPosCallback(_window,  Window::window_mouse_callback);
    glfwSetScrollCallback(_window,  Window::window_scroll_callback);

    std::cout << "OpengGL Context has been set." << std::endl;

    if (_window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, _windowWidth, _windowHeight);
}


void Window::setScene(Scene* scene) {
    _scene = scene;
    _scene->setSceneWidth(_windowWidth);
    _scene->setSceneHeight(_windowHeight);
    _scene->init();
}

Scene* Window::getScene() {
    return _scene;
}

void Window::render()  {
    while(!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        glViewport(0, 0, _windowWidth, _windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _scene->renderScene();
        glfwSwapBuffers(_window);
    }
    _scene->cleanUp();
     glfwTerminate();
}

void Window::window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    getInstance().getScene()->keyCallback(window, key, scancode, action, mode);
}

void Window::window_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    getInstance().getScene()->mouseCallback(window, xpos, ypos);
}

void Window::window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    getInstance().getScene()->scrollCallback(window, xoffset, yoffset);
}

Window &Window::getInstance() {
    static Window instance(WINDOW_WIDTH, WINDOW_HEIGHT, "Best landscape ever.");
    return instance;
}
