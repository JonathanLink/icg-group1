#include "Sun.h"

Sun::Sun() {
    // Do nothing
}

void Sun::init() {
    std::cout << "Init Sun" << std::endl;
    loadShaders( "../src/Sun/sun_vshader.glsl", "../src/Sun/sun_fshader.glsl" );

    glGenVertexArrays(1, &_vertexArrayId);
}


void Sun::render(const glm::mat4 &view, const glm::mat4 &projection) {
    useShaders();

    // Set uniform variables for the vertex and fragment glsl files
    _scene->setUniformVariables(_pid, _model, view, projection);

    glBindVertexArray(_vertexArrayId); // bind VAO
    GLint positionLoc = glGetUniformLocation(_pid, "position");
    glUniform3f(positionLoc, _position.x, _position.y, _position.z);

    // Draw the container
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisable(GL_PROGRAM_POINT_SIZE);
    glBindVertexArray(0);
}

void Sun::cleanUp() {
    std::cout << "CleanUp Sun" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteProgram(_pid);
}

void Sun::setPosition(const glm::vec3 position) {
    _position = position;
}
