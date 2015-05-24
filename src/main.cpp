#include "Window/Window.h"
#include "Scene/MyWorld.h"
#include "Constants/Constants.h"

int main() {
    Window& window = Window::getInstance();

    MyWorld myWorld = MyWorld(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);

    window.setScene(&myWorld);
    window.render();

    return 0;
}
