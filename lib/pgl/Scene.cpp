#include "pgl/Scene.h"

Scene::Scene() : camera(glm::vec3(-0.967917f, 3.54413f, -1.45086f),
                        glm::vec3(-22.4157f, 36.1665f, 0.0f)) {
    keys.resize(1024, false);
    _cameraMode = FLY;
}

void Scene::renderScene() {
    deltaTime = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    updateCameraPosition();

    switch (_cameraMode) {
        case FLY:
            view = camera.getViewMatrix();
            break;
        case FPS:
            break;
         case BEZIER:
            //view = _cameraBezier.getViewMatrix();
            break;
    }

    //projection = glm::perspective(camera.getZoom(), (GLfloat)getSceneAspectRatio(), 0.1f, 100.0f);
    projection = glm::perspective(45.0f, (GLfloat)getSceneAspectRatio(), 0.1f, 200.0f);

    render();
}

void Scene::keyCallback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mode*/) {
    if(action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;  
    }
}

void Scene::mouseCallback(GLFWwindow* /*window*/, double xpos, double ypos) {
    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xOffset = xpos - lastX;
    GLfloat yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.rotate(xOffset, yOffset);
}

void Scene::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.zoom(yoffset);
}

void Scene::setSceneWidth(GLuint width) {
    _sceneWidth = width;
}

void Scene::setSceneHeight(GLuint height) {
    _sceneHeight = height;
}

GLuint Scene::getSceneAspectRatio() const {
    return _sceneWidth / _sceneHeight;
}

void Scene::updateCameraPosition() {
    if(keys[GLFW_KEY_W]) {
        camera.translate(Camera::FORWARD, deltaTime);
    }
    if(keys[GLFW_KEY_S]) {
        camera.translate(Camera::BACKWARD, deltaTime);
    }
    if(keys[GLFW_KEY_A]) {
        camera.translate(Camera::LEFT, deltaTime);
    }
    if(keys[GLFW_KEY_D]) {
        camera.translate(Camera::RIGHT, deltaTime);
    }


}

 Camera Scene::getCamera() {
    return camera;
 }
 
