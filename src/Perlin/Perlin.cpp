#include <iostream>
#include "Perlin.h"

Perlin::Perlin() {
    // Do nothing
}

void Perlin::init() {
    std::cout << "Init Perlin" << std::endl;
    loadShaders( "../src/Perlin/perlin_vshader.glsl", "../src/Perlin/perlin_fshader.glsl" );

    GLfloat vertices[] = { /*V1*/ -1.0f, -1.0f, 0.0f,
                           /*V2*/ +1.0f, -1.0f, 0.0f,
                           /*V3*/ -1.0f, +1.0f, 0.0f,
                           /*V4*/ +1.0f, +1.0f, 0.0f };

    glGenVertexArrays(1, &_vertexArrayId);
    glGenBuffers(1, &_vertexBufferId);

    glBindVertexArray(_vertexArrayId); // bind VAO

        // vertices
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // position attribute (location=0)
        glEnableVertexAttribArray(0);

    glBindVertexArray(0); //unbind VAO
}   

void Perlin::render(const glm::mat4 &view, const glm::mat4 &projection) {
    useShaders();

    // Draw the container
    glBindVertexArray(_vertexArrayId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0); 
}

void Perlin::cleanUp() {
    std::cout << "CleanUp Perlin" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteProgram(pid);
}

