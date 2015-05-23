#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"

#include "../Constants/Constants.h"
#include "Skybox.h"

Skybox::Skybox() {
    // Do nothing
}


void Skybox::init() {
    std::cout << "Init Skybox" << std::endl;

    loadShaders( "../lib/pgl/Skybox/skybox_vshader.glsl", "../lib/pgl/Skybox/skybox_fshader.glsl" );

    GLfloat vertices[] = {
        // Positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
   
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    GLuint VBO;
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    std::vector<const GLchar*> faces;
    faces.push_back("../tex/skybox/right.jpg");
    faces.push_back("../tex/skybox/left.jpg");
    faces.push_back("../tex/skybox/top.jpg");
    faces.push_back("../tex/skybox/bottom.jpg");
    faces.push_back("../tex/skybox/back.jpg");
    faces.push_back("../tex/skybox/front.jpg");
    _cubemapTextureId = loadCubemap(faces);

    // scale the skybox
    model = glm::translate(model, Constants::SKYBOX_TRANSLATION);
    model = glm::scale(model, glm::vec3(Constants::SKYBOX_SCALE, Constants::SKYBOX_SCALE, Constants::SKYBOX_SCALE));
}

void Skybox::render(const glm::mat4 &view, const glm::mat4 &projection) {
    glDepthMask(GL_FALSE);// Remember to turn depth writing off
    
    useShaders();

    scene->setUniformVariables(pid, model, view, projection); 

    // skybox cube
    glBindVertexArray(_VAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(pid, "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapTextureId);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}

void Skybox::cleanUp() {
    std::cout << "CleanUp Skybox" << std::endl;
    glDeleteBuffers(1, &_VBO);
    glDeleteVertexArrays(1, &_VAO);
    glDeleteProgram(pid);
}

GLuint Skybox::loadCubemap(std::vector<const GLchar*> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, n;
    unsigned char* image;
    
    for(GLuint i = 0; i < faces.size(); i++) {
        image = stbi_load(faces[i], &width, &height, &n, STBI_rgb);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}


