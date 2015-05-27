#include <iostream>
#include "Perlin.h"

const char * _params[] = { "LACUNARITY", "FREQ", "OCTAVES", "AMPLITUDE", "X", "FRAMEBUFFER" }; 

const int FRAMEBUFFER_SIZE_DEFAULT_VALUE = 512;
const float LACUNARITY_DEFAULT_VALUE = 1.5;
const float FREQ_DEFAULT_VALUE = 5;
const int OCTAVES_DEFAULT_VALUE = 5;
const float AMPLITUDE_DEFAULT_VALUE = 0.8;
const int X_DEFAULT_VALUE = 0;


Perlin::Perlin() {
    // Do nothing
}

void Perlin::init() {
    std::cout << "Init Perlin" << std::endl;

    loadShaders( "../src/Perlin/perlin_vshader.glsl", "../src/Perlin/perlin_fshader.glsl" );

    _lacunarity = LACUNARITY_DEFAULT_VALUE;
    _freq = FREQ_DEFAULT_VALUE;
    _octaves = OCTAVES_DEFAULT_VALUE;
    _amplitude = AMPLITUDE_DEFAULT_VALUE;
    _x = X_DEFAULT_VALUE;
    _frameBufferWidth = FRAMEBUFFER_SIZE_DEFAULT_VALUE;

    GLfloat vertices[] = { /*V1*/ -1.0f, -1.0f, 0.0f,
                           /*V2*/ +1.0f, -1.0f, 0.0f,
                           /*V3*/ -1.0f, +1.0f, 0.0f,
                           /*V4*/ +1.0f, +1.0f, 0.0f };

    glGenVertexArrays(1, &_vertexArrayId);
    glGenBuffers(1, &_vertexBufferId);

    glBindVertexArray(_vertexArrayId); // bind VAO

        // vertices
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // position attribute (location=0)
        glEnableVertexAttribArray(0);

    glBindVertexArray(0); //unbind VAO


}  

void Perlin::keyCallback(int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS && key == GLFW_KEY_P) {
        _perlinModeIsEnabled =! _perlinModeIsEnabled;
        std::cout << "PERLIN MODE  = " << _perlinModeIsEnabled << std::endl;
        _param = (ParamNoise)0;
        std::cout << "[PERLIN MODE] PARAM = " << _params[_param] << std::endl;

    }

    if (_perlinModeIsEnabled) {
        float value = 0;
        float step = 0;

        if (action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
            _param = (ParamNoise)((_param + 1) % 6);
            std::cout << "[PERLIN MODE] PARAM = " << _params[_param] << std::endl;
        } else if (action == GLFW_PRESS && key == GLFW_KEY_R) {
            std::cout << "[PERLIN MODE] RESET PARAMS" << std::endl;
            _lacunarity = LACUNARITY_DEFAULT_VALUE;
            _freq = FREQ_DEFAULT_VALUE;
            _octaves = OCTAVES_DEFAULT_VALUE;
            _amplitude = AMPLITUDE_DEFAULT_VALUE;
            _x = X_DEFAULT_VALUE;
            _frameBufferWidth = FRAMEBUFFER_SIZE_DEFAULT_VALUE;
        } else if (action == GLFW_PRESS && key == GLFW_KEY_O) {
            std::cout << "[PERLIN MODE] VALUES:" << std::endl;
            std::cout << "\t lacunarity = " << _lacunarity << std::endl;
            std::cout << "\t freq = " << _freq << std::endl;
            std::cout << "\t octaves = " << _octaves << std::endl;
            std::cout << "\t amplitude = " << _amplitude << std::endl;
            std::cout << "\t x = " << _x << std::endl;
            std::cout << "\t framebuffer = " << _frameBufferWidth << std::endl;
        }

        switch(_param) {
            case LACUNARITY: {
                value = _lacunarity;
                step = 0.05;
                break;
            }

            case FREQ: {
                value = _freq;
                step = 0.05;
                break;
            }

            case OCTAVES: {
                value = _octaves;
                step = 1;
                break;
            }

            case AMPLITUDE: {
                value = _amplitude;
                step = 0.05;
                break;
            }

            case X: {
                value = _x;
                step = 0.5;
                break;
            }


            case FRAMEBUFFER: {
                value = _frameBufferWidth;
                step = 100;
                break;
            }
        }

        if (action == GLFW_PRESS && key ==  GLFW_KEY_UP) {
            value = value + step;
        } else if (action == GLFW_PRESS && key ==  GLFW_KEY_DOWN) {
            value = value - step;
        }

        switch(_param) {
            case LACUNARITY: {
                _lacunarity =  value;
                break;
            }

            case FREQ: {
                _freq =  value;
                break;
            }
            
            case OCTAVES: {
                _octaves = value;
                break;
            }
           
            case AMPLITUDE: {
                _amplitude = value;
                break;
            }

            case X: {
                _x = value;
                break;
            }

            case FRAMEBUFFER: {
                _frameBufferWidth = value;
                break;
            }
        }
    }
} 

void Perlin::render(const glm::mat4 &view, const glm::mat4 &projection) {
    useShaders();

    glUniform1f(glGetUniformLocation(_pid, "_lacunarity"), _lacunarity);
    glUniform1f(glGetUniformLocation(_pid, "_freq"), _freq);
    glUniform1i(glGetUniformLocation(_pid, "_octaves"), _octaves);
    glUniform1f(glGetUniformLocation(_pid, "_amplitude"), _amplitude);
    glUniform1f(glGetUniformLocation(_pid, "_x"), _x);

    // Draw the container
    glBindVertexArray(_vertexArrayId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0); 
}

void Perlin::cleanUp() {
    std::cout << "CleanUp Perlin" << std::endl;
    glDeleteVertexArrays(1, &_vertexArrayId);
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteProgram(_pid);
}

bool Perlin::isPerlinModeIsEnabled() {
    return _perlinModeIsEnabled;
}

int Perlin::getFrameBufferWidth() {
    return _frameBufferWidth;
}

