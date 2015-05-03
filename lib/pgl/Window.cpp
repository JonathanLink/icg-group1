#include "pgl/Window.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); 
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

static GLuint _windowWidth;
static GLuint _windowHeight;
static const char* _title;
static GLFWwindow* _window;
static Scene* _scene;

void Window::init(GLuint width, GLuint height, const char* title) {

    _windowWidth = width;
    _windowHeight = height;
    _title = title;


    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for MacOSX compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, NUMBER_OF_SAMPLES_FOR_MULTISAMPLING); // for antialiasing 
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  

    _window = glfwCreateWindow(_windowWidth, _windowHeight, _title, nullptr, nullptr);

    glfwMakeContextCurrent(_window);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    // set  callback functions
    glfwSetKeyCallback(_window, key_callback);
    glfwSetCursorPosCallback(_window, mouse_callback);
    glfwSetScrollCallback(_window, scroll_callback);


    std::cout << "OpengGL Context has been set." << std::endl;

    if (_window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    // glew init
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return;
    }


    // OpenGL settings
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    Window::getScene()->keyCallback(window, key, scancode, action, mode);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Window::getScene()->mouseCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Window::getScene()->scrollCallback(window, xoffset, yoffset);
}



