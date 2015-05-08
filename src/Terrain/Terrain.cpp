#include "Terrain.h"
#include "../Constants.h"


Terrain::Terrain() {
    // Do nothing
}

void Terrain::init() {
    std::cout << "Init Terrain" << std::endl;
    loadShaders( "../src/Terrain/terrain_vshader.glsl", "../src/Terrain/terrain_fshader.glsl" );

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
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));


    // Bind Grass Texture
    _grassTexId = gen2DTexture("../tex/grass.jpg", GL_RGB);
     // Bind Snow Texture
    _snowTexId = gen2DTexture("../tex/snow.jpg", GL_RGB);
     // Bind Sand Texture
    _sandTexId = gen2DTexture("../tex/sand.jpg", GL_RGB);
     // Bind Rock Texture
    _rockTexId = gen2DTexture("../tex/rock.jpg", GL_RGB);
}   


void Terrain::render(const glm::mat4 &view, const glm::mat4 &projection) {
   useShaders();

    // Set uniform variables for the vertex and fragment glsl files
    GLuint modelViewLoc_id = glGetUniformLocation(pid, "modelView");
    glm::mat4 modelView = view * model;
    glUniformMatrix4fv(modelViewLoc_id, 1, GL_FALSE,  glm::value_ptr(modelView));

    GLuint mvpLoc_id = glGetUniformLocation(pid, "MVP_matrix");
    glm::mat4 mvp = projection * modelView;
    glUniformMatrix4fv(mvpLoc_id, 1, GL_FALSE,  glm::value_ptr(mvp));

    GLuint grid_size_id = glGetUniformLocation(pid, "grid_size");
    glm::float1 grid_size = (float)GRID_SIZE;
    glUniform1f(grid_size_id, grid_size);


    /* Bind textures */
    //Perlin Noise
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    GLuint tex_id = glGetUniformLocation(pid, "tex");
    glUniform1i(tex_id, 0);
    //Grass
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _grassTexId);
    glUniform1i(glGetUniformLocation(pid, "grassTex"), 1);
    //Snow
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _snowTexId);
    glUniform1i(glGetUniformLocation(pid, "snowTex"), 2);
    //Sand
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _sandTexId);
    glUniform1i(glGetUniformLocation(pid, "sandTex"), 3);
    //Rock
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _rockTexId);
    glUniform1i(glGetUniformLocation(pid, "rockTex"), 4);


    // Draw
    glBindVertexArray(_vertexArrayId);
    glDrawElements(GL_TRIANGLE_STRIP, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Terrain::cleanUp() {
    std::cout << "CleanUp Terrain" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteBuffers(1, &_elementBufferId);
    glDeleteTextures(1, &_textureId);
    glDeleteProgram(pid);
}

void Terrain::constructGrid() {
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

void Terrain::setTexture(GLuint textureId) {
    _textureId = textureId;
}



