#include "pgl/common.h"
#include "pgl/Window.h"
#include "MyWorld.h"

#include "Constants.h"

int main() {
    Window::init(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT,
                 "The best landscape ever!");

    MyWorld myWorld = MyWorld();

    Window::setScene(&myWorld);
    Window::render();

    return 0;
}
