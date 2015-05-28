#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"

#include "../Constants/Constants.h"

#include "Water.h"


Water::Water() {
    // Do nothing
}

void Water::init() {
    std::cout << "Init Water" << std::endl;
    loadShaders( "../src/Water/water_vshader.glsl", "../src/Water/water_fshader.glsl" );

    constructGrid();
    glGenVertexArrays(1, &_vertexArrayId);
    glGenBuffers(1, &_vertexBufferId);
    glGenBuffers(1, &_elementBufferId);
    glBindVertexArray(_vertexArrayId);
        // vertices
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices[0], GL_STATIC_DRAW);
        // position attribute (location=0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), &_indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);

    // Apply a rotation on the model matrix
    //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    _model = glm::scale(_model, glm::vec3(Constants::TERRAIN_SCALE,
                                        Constants::TERRAIN_SCALE,
                                        Constants::TERRAIN_SCALE));

    // Bind Grass Texture
    _waterTextureId = gen2DTexture("../tex/water4.jpg", GL_RGBA);


    /* 
    PROBLEM
        Find a better normalMap Texture for water because we can see shading artefact right now
        Maybe the problem is not related to the actual texture but the way it is stored or
        the way the diffuse term is calculated
    PROBLEM
    */
    _waterNormalTextureId = gen2DTexture("../tex/waternormal3.jpg", GL_RGB);
    
}   


void Water::render(const glm::mat4 &view, const glm::mat4 &projection) {
    useShaders();

    // Set uniform variables for the vertex and fragment glsl files
    _scene->setUniformVariables(_pid, _model, view, projection);

    // grid size uniform
    GLuint grid_size_id = glGetUniformLocation(_pid, "grid_size");
    glm::float1 grid_size = (float)GRID_SIZE;
    glUniform1f(grid_size_id, grid_size);

    // water height uniform
    GLuint water_height_id = glGetUniformLocation(_pid, "water_height");
    //Todo constante a FIXER
    glUniform1f(water_height_id, Constants::WATER_HEIGHT);

    //time uniform
    GLuint time_id = glGetUniformLocation(_pid, "time");
    glm::float1 time_size = _scene->getReflectTime();
    glUniform1f(time_id, time_size);

    /* Bind textures */
    //Perlin texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _perlinTextureId);
    glUniform1i(glGetUniformLocation(_pid, "tex"), 0);

    //Terrain reflection
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _mirrorTextureId);
    glUniform1i(glGetUniformLocation(_pid, "tex_mirror"), 1);

    //Terrain reflection
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _waterTextureId);
    glUniform1i(glGetUniformLocation(_pid, "tex_water"), 2);

    //Water normal
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _waterNormalTextureId);
    glUniform1i(glGetUniformLocation(_pid, "tex_water_normal"), 3);

    // Draw
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(_vertexArrayId);
    glDrawElements(GL_TRIANGLE_STRIP, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void Water::cleanUp() {
    std::cout << "CleanUp Water" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteBuffers(1, &_elementBufferId);
    glDeleteTextures(1, &_perlinTextureId);
    glDeleteTextures(1, &_mirrorTextureId);
    glDeleteProgram(_pid);
}

void Water::constructGrid() {
    const double STEP = (1.0 / GRID_SIZE) * 2.0;

    // construct vertices grid
    Point points [GRID_SIZE + 1][GRID_SIZE + 1];
    Point point;
    point.y = 1.0f;
    int currentIndice = 0;
    point.indice = currentIndice;
    for (int row = 0; row <= GRID_SIZE; ++row) {
        point.x = -1.0f;
        for (int col = 0; col <= GRID_SIZE; ++col) {
            points[row][col] = point;
            _vertices.push_back(point.x);
            _vertices.push_back(point.y);
            point.x = point.x + STEP;
            point.y = point.y;
            currentIndice = currentIndice + 1;
            point.indice = currentIndice;
        }
        point.y = point.y - STEP;
    }

    // bind triangles
    int primitive_restart_idx = 0xffffffff;
    glPrimitiveRestartIndex(primitive_restart_idx);
    glEnable(GL_PRIMITIVE_RESTART);

    for (int row = 0; row <= GRID_SIZE - 1  ; ++row ) {
        for (int col = 0; col <= GRID_SIZE; ++col) {
            Point point;
            point = points[row][col];
            _indices.push_back(point.indice);
            point = points[row + 1][col];
            _indices.push_back(point.indice);
            if (col == GRID_SIZE) {
                _indices.push_back(primitive_restart_idx);
                point = points[row + 1][0];
                _indices.push_back(point.indice);
            }
        }
    }
}

void Water::setTexturePerlin(GLuint textureId) {
    _perlinTextureId = textureId;
}

void Water::setTextureMirror(GLuint textureId) {
    _mirrorTextureId = textureId;
}

