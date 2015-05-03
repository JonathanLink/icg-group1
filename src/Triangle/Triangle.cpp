#include "Triangle.h"

#include <cmath>

Triangle::Triangle()  {

}

void Triangle::init() {
    std::cout << "Init Triangle" << std::endl;
    

    glGenVertexArrays(1, &_vertexArrayId);
    glBindVertexArray(_vertexArrayId);

    loadShaders( "../src/Triangle/triangle_vshader.glsl", "../src/Triangle/triangle_fshader.glsl" );

    static const GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };


    glBindVertexArray(_vertexArrayId); // bind VAO
    glGenBuffers(1, &_vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

     // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)( 3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); //unbind VAO


}   

void Triangle::render(const glm::mat4 &view, const glm::mat4 &projection) {

    // activate the shader
    useShaders();

    // Set shader triangle uniform variables
    GLfloat timeValue = glfwGetTime();
    GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
    GLint vertexColorLocation = glGetUniformLocation(pid, "myColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // Draw the triangle
    glBindVertexArray(_vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0); 
}

void Triangle::cleanUp() {
    std::cout << "CleanUp Triangle" << std::endl;
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteProgram(pid);
}
