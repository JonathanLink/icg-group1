#include "pgl/Window.h"
#include "MyWorld.h"

int main() {
    Window& window = Window::getInstance();
    
    MyWorld myWorld = MyWorld();

    window.setScene(&myWorld);
    window.render();

    return 0;
}
