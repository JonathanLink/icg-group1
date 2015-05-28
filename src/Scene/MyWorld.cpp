#include "../Constants/Constants.h"
#include "MyWorld.h"
#include "../Window/Window.h"

MyWorld::MyWorld(unsigned int terrainReflectFbWidth,
                 unsigned int terrainReflectFbHeight):
        Scene(glm::vec3(-0.967917f, 20.54413f, -1.45086f),
              glm::vec3(-22.4157f, 36.1665f, 0.0f)),
        _terrainReflectFB(terrainReflectFbWidth, terrainReflectFbHeight) {
    // Do nothing
}

void MyWorld::init() {
    _skybox.setScene(this);
    _perlin.setScene(this);
    _terrain.setScene(this);
    _water.setScene(this);
    _particles.setScene(this);
    _bezierController.setScene(this);
    _sun.setScene(this);

    drawPerlin();
    _wireframeIsEnabled = false;
}

void MyWorld::drawPerlin() {
    // Draw perlin noise in framebuffer we've just created
    FrameBuffer perlinFrameBuffer = FrameBuffer(_perlin.getFrameBufferWidth(), _perlin.getFrameBufferWidth());
    _perlinTextureId = perlinFrameBuffer.initTextureId(GL_R32F); 
    perlinFrameBuffer.bind();
        _perlin.render(_view, _projection);
    perlinFrameBuffer.unbind();

    // Save height map in memory
    _heightMap = new float[_perlin.getFrameBufferWidth() * _perlin.getFrameBufferWidth()];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _perlinTextureId);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, _heightMap);

    _water.setTexturePerlin(_perlinTextureId);
    _terrain.setTexture(_perlinTextureId);
}

void MyWorld::render() {
    //Draw terrain in framebuffer for water reflection
    if (true) { // to debug perlin purpose
        if (_perlin.isPerlinModeIsEnabled()) {
            drawPerlin();
        }

        GLuint terrainReflectTextureId = _terrainReflectFB.initTextureId(GL_RGB);
        _terrainReflectFB.bind();
            _terrain.setReflection(true);
            _terrain.render(_view, _projection);
            _terrain.setReflection(false);
        _terrainReflectFB.unbind();
        _water.setTextureMirror(terrainReflectTextureId);

        if (_wireframeIsEnabled) {
             glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // wireframe
         } else {
             glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
         }
        _skybox.render(_view, _projection);
        _terrain.render(_view, _projection);
        _water.render(_view, _projection);

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

         _terrainReflectFB.cleanUp();
        if (_particlesEnabled) {
            _particles.render(_view, _projection);
        }

        if (_bezierController.bezierEditModeEnabled) {
            _bezierController.render(_view, _projection);
        }

        _sun.setPosition(getLightPosition());
        _sun.render(_view, _projection);
    } else {
        _perlin.render(_view, _projection);
    }
}

void MyWorld::cleanUp() {
    _skybox.cleanUp();
    _terrain.cleanUp();
    _perlin.cleanUp();

    _bezierController.cleanUp();

    _particles.cleanUp();
    _sun.cleanUp();
}

// This method is ugly and i know it!
void MyWorld::keyCallback(int key, int scancode, int action, int mode) {
    _bezierController.keyCallback(key, scancode, action, mode);

    if (action == GLFW_PRESS && key == GLFW_KEY_R) {
        _particlesEnabled = ! _particlesEnabled;
        if (_particlesEnabled) {
            std::cout << "Particles mode enabled" << std::endl;
        } else {
            std::cout << "Particles mode disabled" << std::endl;
        }

    }

   if (!_bezierController.bezierEditModeEnabled) {
        _perlin.keyCallback(key, scancode, action, mode);
   }

   if (action == GLFW_PRESS && key == GLFW_KEY_L) {
        _wireframeIsEnabled = !_wireframeIsEnabled;
   }

    // *********************************
    // ****bezier ajust speed ****
    // *********************************
    if (action == GLFW_PRESS && key == GLFW_KEY_J) {
        _cameraBezier.decreaseSpeed();
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_K) {
        _cameraBezier.increaseSpeed();
    }
}

void MyWorld::updateFpsCameraPosition() {
    // We use fly through camera first, and then put a constraint on Y coordinate
    updateFlyCameraPosition();
    glm::vec3 cameraPosition = _camera.getPosition();
    glm::vec2 pos_2d(cameraPosition.x, cameraPosition.z);

    // Convert world coordinates to texture coordinates
    glm::vec2 pos_texture(
        (pos_2d.x / Constants::TERRAIN_SCALE + 1.0) * 0.5,
        (pos_2d.y / Constants::TERRAIN_SCALE + 1.0) * 0.5
    );
    //std::cout << "2D position: " << pos_2d.x << ", " << pos_2d.y << std::endl;
    //std::cout << "Texture position: " << pos_texture.x << ", " << pos_texture.y << std::endl;

    // Find height corresponding to current position in texture scale
    float normalizedHeight = getHeight(pos_texture.x * _perlin.getFrameBufferWidth(), pos_texture.y * _perlin.getFrameBufferWidth());
    //std::cout << "Height: " << normalizedHeight << std::endl;

    // Find height corresponding to current position in world scale, with a small offset in Y direction.
    float height = (normalizedHeight + 0.05) * Constants::TERRAIN_SCALE;
    if(_keys[GLFW_KEY_SPACE] && !_hasJumped) {
        _hasJumped = true;
        _jumpStartTime = glfwGetTime();
        _jumpStartHeight = height;
    }

    if (_hasJumped) {
        GLfloat timeSinceJump = glfwGetTime() - _jumpStartTime;
        const static float MAX_JUMP_TIME = 3.0f;
        if (timeSinceJump <= MAX_JUMP_TIME) {
            // Change Y position according to parabola of height vs time
            GLfloat jumpHeight = 9 - pow(3 * timeSinceJump - 3, 2.0) + _jumpStartHeight;
            _camera.setHeight(std::max(jumpHeight, height));
        } else {
            _hasJumped = false;
        }
    } else {
        _camera.setHeight(height);
    }
}

float MyWorld::getHeight(unsigned int x, unsigned int y) {
    unsigned int width = _perlin.getFrameBufferWidth();
    unsigned int height = _perlin.getFrameBufferWidth();
    if (x < width && y < height) {
        return _heightMap[x + _perlin.getFrameBufferWidth() * y];
    } else {
        return 0.0;
    }
}
