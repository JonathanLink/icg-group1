#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "common.h"

class FrameBuffer {
public:
    FrameBuffer(GLuint width, GLuint height);
    GLuint initTextureId();
    void bind();
    void unbind();

private:
    GLuint _frameBufferObject;
    GLuint _width;
    GLuint _height;

    GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
};

#endif