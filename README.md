README
==================================
# Keyboard tips:

Press:

* C to change camera (FLY, FPS, BEZIER)
* W,A,S,D + mouse in FLY and FPS mode	
* SPACE BAR to jump in FPS mode
* R to turn on/off the rain
* F to turn on/off the fog
* L to turn on/off wireframe mode
* J to increase camera bezier speed
* K to decrease camera bezier speed

# Bezier Mode:

Press:

* B to enter Bezier mode
* U to select the first handle
* I to select the second handle
* O to select the third handle
* P to select the forth and last handle
* X to move along the X-axe
* Y to move along the Y-axe
* Z to move along the z-axe
* UP ARROW to increase value
* DOWN ARROW to decrease value
* LEFT ARROW to increase step value
* RIGHT ARROW to decrease step value
* ENTER to write the 4 handles coordinates into the console

# Perlin Mode:

Press:

* P to enter Bezier mode
* SPACE BAR to select which perlin parameter you want to modify
* UP ARROW to increase value
* DOWN ARROW to decrease value
* O to write all bezier parameters into the console
* R to reset the terrain to default bezier parameters

# Code:
Description of the root directory items:

* include
   * Contains all external libs headers
* lib
   * Contains all external libs
* src
   * Contains source of the project
* tex
   * Contains all textures of the project


# How to build:

* Make sure to have OpenGL, GLEW, GLFW3 and PkgConfig.

## Mac OSX install procedure (with brew):

GLEW:
`brew install glew`

GLFW3:
`brew tap homebrew/versions`
`brew install glfw3`

PkgConfig:
`brew install pkg-config`

Build process
* `cd myWorld`
* `mkdir build`
* `cd build`
* `cmake ..`
* To compile and run the code
    * `make`
    * `./myWorld`

# PGL Architecture:

Description of all classes of the pgl library:

* Window:
    * It is a static class. For the moment, pgl can only handle one and unique window.

* Scene
    * The window need to be associated to a Scene in order to display something.
    * A Scene is an abstract class which:
        * will handle all keyboard/mouse input
        * set a default FPS camera for the scene
        * force its subclass to implement those 3 methods: init(), render(), cleanUp().
    * ADVICE: see Scene header to have a complete overview of the class.

* RenderObject
    * Each object you want to render MUST be a subclass of RenderObject. That way our class is force to implement those 3 methods: init(), render(mat4 view, mat4 projection) and cleanUp().
    * IMPORTANT: In the init method of your subclass of Scene, don’t forget to call setScene(this);
    RenderObject offers a method to load your vertex and fragments shader and one method to generate a 2D texture.
    * ADVICE: See RenderObject header to have a complete overview of the class.

* Camera:
    * Default camera of the pgl library. You never have to do anything with this class. 
    * Scene object handle it for you.

* FrameBuffer
    * FrameBuffer object with bind and unbind functions. Draw your texture in between those 2 calls.
    * ADVICE: See FrameBuffer header to have a complete overview of the class.


> ********************************************************************************
> FOR A BETTER UNDERSTANDING, HAVE A LOOK AT THOSE SOURCE FILES: 
> src/main.cpp, src/MyWorld.cpp, src/Terrain/Terrain.cpp
> ********************************************************************************

# PoGL Background

A small home-made library aiming to accelerate simple openGL projects or for prototyping purpose. PoGL actually stands for Prototyping OpenGL (name is subject to change, still brainstorming, fist name was pgl)

