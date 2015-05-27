#include "Particles.h"
#include "../Constants/Constants.h"

Particles::Particles() {
    srand (time(NULL));
}

float Particles::randomNumber(float minimum, float maximum) {
    return ((float(rand()) / float(RAND_MAX)) * (maximum - minimum)) + minimum;
}

float Particles::randomOffset() {
    return randomNumber(-5.0f, 5.0f);
}

void Particles::init() {
    std::cout << "Init Particles" << std::endl;
    loadShaders( "../src/Particles/particles_vshader.glsl", "../src/Particles/particles_fshader.glsl" );

    _vertices = {
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f
    };

    glGenVertexArrays(1, &_vertexArrayId);
    glGenBuffers(1, &_vertexBufferId);

    glBindVertexArray(_vertexArrayId);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices[0], GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

    glBindVertexArray(0);


    const int particleHalfGridSize = ceil(Constants::TERRAIN_SCALE / SCALE_FACTOR);
    const float layerCount = ceil(SCALE_FACTOR * 4.0f);
    for (int i = -particleHalfGridSize; i < particleHalfGridSize; ++i) {
        for (int j = -particleHalfGridSize; j < particleHalfGridSize; ++j) {
            for (int k = 0; k < layerCount; ++k) {
                _particlePositions.push_back(glm::vec3(
                        i * SCALE_FACTOR + randomOffset(), MAX_HEIGHT - k * SCALE_FACTOR + randomOffset(), j * SCALE_FACTOR + randomOffset()
                ));
                _particlePositions.push_back(glm::vec3(
                        i * SCALE_FACTOR + randomOffset(), MAX_HEIGHT + (k + 1) * SCALE_FACTOR + randomOffset(), j * SCALE_FACTOR + randomOffset()
                ));
            }
        }
    }

    _initialTime = glfwGetTime();
}

void Particles::render(const glm::mat4 &view, const glm::mat4 &projection) {
    useShaders();

    _scene->setUniformVariables(_pid, _model, view, projection);

    double currentTime = glfwGetTime() - _initialTime;
    if (currentTime >= DESCENT_TIME) {
        _initialTime = glfwGetTime();
    }
    const float slope = (MAX_HEIGHT - 0.0f) / (DESCENT_TIME - 0.0f);
    GLfloat delta = slope * currentTime;

    GLuint modelLoc = glGetUniformLocation(_pid, "current_model");
    glBindVertexArray(_vertexArrayId);
    glEnable(GL_BLEND);
    for (GLuint i = 0; i < _particlePositions.size(); i++) {
        glm::mat4 model2;
        model2 = glm::translate(model2, _particlePositions[i]);
        model2 = glm::translate(model2, glm::vec3(0.0, -delta, 0.0));
        // Transformation
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        glDrawArrays(GL_LINE_STRIP, 0, _vertices.size());
    }
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}

void Particles::cleanUp() {
    std::cout << "CleanUp Particles" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteProgram(_pid);
}
