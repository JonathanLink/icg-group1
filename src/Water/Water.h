#ifndef WATER_H
#define WATER_H

#include "../Scene/RenderObject.h"

class Water: public RenderObject {
public:
    Water();
    void init() override;
    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void cleanUp() override;
    void setTexturePerlin(GLuint textureId);
    void setTextureMirror(GLuint textureId);

private:
    GLuint _vertexArrayId;
    GLuint _vertexBufferId;
    GLuint _elementBufferId;
    GLuint _perlinTextureId;
    GLuint _mirrorTextureId;
    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _indices;

    static const int GRID_SIZE = 512;

    struct Triangle {
        double x;
        double y;
        double z;
    };
    struct Point {
        double x;
        double y;
        double indice;
    };

    void constructGrid();
};

#endif