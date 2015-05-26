#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Curve.h"

Curve::Curve() {
    // Do nothing
}

void Curve::init() {
    std::cout << "Init Curve" << std::endl;
    loadShaders( "../src/Bezier/Curve/curve_vshader.glsl", "../src/Bezier/Curve/curve_fshader.glsl" );

    glGenVertexArrays(1, &_vertexArrayId);
    glGenBuffers(1, &_vertexBufferId);

    _isLookAtCurve = false;
}   


void Curve::render(const glm::mat4 &view, const glm::mat4 &projection) {
   
    useShaders();

    // Set uniform variables for the vertex and fragment glsl files
    _scene->setUniformVariables(_pid, _model, view, projection);

    // lookAtCurve
    GLint isLookAtCurveLoc = glGetUniformLocation(_pid, "isLookAtCurve");
    int lookAtCurveValue = (_isLookAtCurve) ? 1 : 0 ;
    glUniform1i(isLookAtCurveLoc, lookAtCurveValue);  

    // Draw the container
    glBindVertexArray(_vertexArrayId);
    glDrawArrays(GL_LINE_STRIP, 0, _points.size());
    glBindVertexArray(0); 
}

void Curve::cleanUp() {
    std::cout << "CleanUp Curve" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteProgram(_pid);
}

void Curve::setPoints(std::vector<glm::vec3> points) {
    _points = points;
    
    glBindVertexArray(_vertexArrayId); // bind VAO

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _points.size(), &_points[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);  // position attribute (location=0)
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); //unbind VAO

}

void Curve::setIsLookAtCurve(bool isLookAtCurve) {
    _isLookAtCurve = isLookAtCurve;
}



