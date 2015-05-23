#include "Window/Window.h"
#include "Scene/MyWorld.h"

int main() {
    Window& window = Window::getInstance();
    
    MyWorld myWorld = MyWorld();

    window.setScene(&myWorld);
    window.render();

    return 0;
}
