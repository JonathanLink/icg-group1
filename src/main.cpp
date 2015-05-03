#include "pgl/common.h"
#include "pgl/Window.h"
#include "MyWorld.h"

static const GLuint WINDOW_WIDTH = 800;
static const GLuint WINDOW_HEIGHT = 600;

int main() {

    Window::init(WINDOW_WIDTH, WINDOW_HEIGHT, "The best landscape ever!");

    MyWorld myWorld = MyWorld();

    Window::setScene(&myWorld);
    Window::render();

    return 0;
}
