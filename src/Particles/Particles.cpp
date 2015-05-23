#include "Particles.h"

Particles::Particles() {
    // Do nothing
}

void Particles::init() {
    std::cout << "Init Particles" << std::endl;
    loadShaders( "../src/Particles/particles_vshader.glsl", "../src/Particles/particles_fshader.glsl" );

    glGenVertexArrays(1, &_vertexArrayId);
    glGenBuffers(1, &_vertexBufferId);
    glGenBuffers(1, &_elementBufferId);

    glBindVertexArray(_vertexArrayId);
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), &_indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Particles::render(const glm::mat4 &view, const glm::mat4 &projection) {
    useShaders();

    scene->setUniformVariables(pid, model, view, projection);

    //time uniform
    GLuint time_id = glGetUniformLocation(pid, "time");
    glm::float1 time_size = glfwGetTime();
    glUniform1f(time_id, time_size);

    glBindVertexArray(_vertexArrayId);
    glDrawElements(GL_TRIANGLE_STRIP, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Particles::cleanUp() {
    std::cout << "CleanUp Terrain" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteBuffers(1, &_elementBufferId);
    glDeleteProgram(pid);
}
