#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

class FrameBuffer {
public:
    FrameBuffer(GLuint width, GLuint height);
    GLuint initTextureId(GLint internalFormat);
    void bind();
    void unbind();
    void cleanUp();

private:
    GLuint _frameBufferObject;
    GLuint _renderBufferObject;
    GLuint _textureColorbuffer;
    GLuint _width;
    GLuint _height;

    GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, GLint internalFormat);
};

#endif