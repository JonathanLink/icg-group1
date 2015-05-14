#include "pgl/FrameBuffer.h"


FrameBuffer::FrameBuffer(GLuint width, GLuint height): _width(width), _height(height) {
    // Do nothing
}

GLuint FrameBuffer::initTextureId(GLint internalFormat) {
    // Framebuffer
    glGenFramebuffers(1, &_frameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);

    // Create a color attchment texture
    GLuint textureColorbuffer = generateAttachmentTexture(false, false, internalFormat);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // Create a renderbuffer object for depth and stencil attachment
    GLuint renderBufferObject;
    glGenRenderbuffers(1, &renderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete :-/" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return textureColorbuffer;
}


void FrameBuffer::bind() {
    glViewport(0, 0, _width, _height);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT); 
}

GLuint FrameBuffer::generateAttachmentTexture(GLboolean depth, GLboolean stencil, GLint internalFormat) {
    GLenum attachmentType;
    if (!depth && !stencil) {
        attachmentType = GL_RGB;
    } else if (depth && !stencil) {
        attachmentType = GL_DEPTH_COMPONENT;
    } else if (!depth && stencil) {
        attachmentType = GL_STENCIL_INDEX;
    } else {
        attachmentType = GL_DEPTH_STENCIL;
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0 , internalFormat, _width, _height, 0, attachmentType, GL_UNSIGNED_BYTE, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}


