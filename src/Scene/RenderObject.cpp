#include "RenderObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

RenderObject::RenderObject() {
    // Do nothing
}

// TODO: better to use that solution: http://stackoverflow.com/questions/11093444/compile-a-program-with-local-file-embedded-as-a-string-variable
void RenderObject::loadShaders(const char* vertexShaderFile,const char* fragmentShaderFile) {
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    try {
        // Open files
        std::ifstream vShaderFile(vertexShaderFile);
        std::ifstream fShaderFile(fragmentShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream into GLchar array
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();     
    } catch(std::exception e) {
        std::cout << "[ERROR] RenderObject:loadShaders()\n\tFILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    // Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "[ERROR] RenderObject:loadShaders()\n\t" << infoLog << std::endl;
    };

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "[ERROR] RenderObject:loadShaders()\n\t" << infoLog << std::endl;
    };

     // Shader Program
    pid = glCreateProgram();
    glAttachShader(pid, vertex);
    glAttachShader(pid, fragment);
    glLinkProgram(pid);

    // Print linking errors if any
    glGetProgramiv(pid, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pid, 512, NULL, infoLog);
        std::cout << "[ERROR] RenderObject:loadShaders()\n\tLINKING_FAILED\n\t" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);


}

void RenderObject::useShaders() {
    glUseProgram(pid); 
}

GLuint RenderObject::gen2DTexture(const char* imagePath, GLint format) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int req_comp = (format == GL_RGB) ? STBI_rgb : STBI_rgb_alpha;

    int width, height, n;
    unsigned char* image = stbi_load(imagePath, &width, &height, &n, req_comp);
    if (image != NULL) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image );
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

void RenderObject::setScene(Scene *s) {
    scene = s;
    init();
}

