#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Square.h"

Square::Square() {
    // Do nothing
}

void Square::init() {
    std::cout << "Init Square" << std::endl;
    loadShaders( "../src/Square/square_vshader.glsl", "../src/Square/square_fshader.glsl" );

    GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
    };

    GLuint indices[] = { 
        0, 1, 3,
        1 , 2, 3
    };  

    glGenVertexArrays(1, &_vertexArrayId);
    glGenBuffers(1, &_vertexBufferId);
    glGenBuffers(1, &_elementBufferId);

    glBindVertexArray(_vertexArrayId); // bind VAO

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // better: 
    // addVertexAttribute(GLenum type, GLsizei sizeType,  GLint size); 
    // addVertexAttribute...
    // enableVertexAttributes();

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)( 3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)( 6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0); //unbind VAO

    // Bind Texture
    _texture1 = gen2DTexture("../tex/container.jpg", GL_RGB);
    _texture2 = gen2DTexture("../tex/awesomeface.png", GL_RGBA);

    // Transformation
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}   

void Square::render(const glm::mat4 &view, const glm::mat4 &projection) {
    // better: 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture1);
    glUniform1i(glGetUniformLocation(this->pid, "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texture2);
    glUniform1i(glGetUniformLocation(this->pid, "ourTexture2"), 1);

    useShaders();

    // Transformation
    // better: setUniformMatrix4fv("model", glm::value_ptr(model));
    GLuint modelLoc = glGetUniformLocation(pid, "model");
    GLuint viewLoc = glGetUniformLocation(pid, "view");
    GLuint projectionLoc = glGetUniformLocation(pid, "projection");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE,  glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE,  glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,  glm::value_ptr(projection));
    // https://www.khronos.org/opengles/sdk/docs/man/xhtml/glUniform.xml

    // Draw the container
    glBindVertexArray(_vertexArrayId);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); 
}

void Square::cleanUp() {
    std::cout << "CleanUp Square" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteBuffers(1, &_elementBufferId);
    glDeleteProgram(pid);
}

