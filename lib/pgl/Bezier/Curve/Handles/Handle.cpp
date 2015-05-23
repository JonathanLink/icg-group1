#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "pgl/Bezier/Curve/Handles/Handle.h"

Handle::Handle() {
    // Do nothing
}

void Handle::init() {
    std::cout << "Init Handle" << std::endl;
    loadShaders( "../lib/pgl/Bezier/Curve/Handles/handle_vshader.glsl", "../lib/pgl/Bezier/Curve/Handles/handle_fshader.glsl" );

    glGenVertexArrays(1, &_vertexArrayId);

}   


void Handle::render(const glm::mat4 &view, const glm::mat4 &projection) {
   
    useShaders();

    // Set uniform variables for the vertex and fragment glsl files
    scene->setUniformVariables(pid, model, view, projection);

    glBindVertexArray(_vertexArrayId); // bind VAO
    GLint positionLoc = glGetUniformLocation(pid, "position");
    glUniform3f(positionLoc, _handle.x, _handle.y, _handle.z);  

    // Draw the container
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisable(GL_PROGRAM_POINT_SIZE);
    glBindVertexArray(0);
}

void Handle::cleanUp() {
    std::cout << "CleanUp Handle" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteProgram(pid);
}

void Handle::setHandle(const glm::vec3 &handle) {
    _handle = handle;
}


