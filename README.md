README
==================================

Description of the root directory items:

* include
   * Contains all external libs headers
* lib
   * Contains all external libs
* src
   * Contains source of the project
* tex
   * Contains all textures of the project


> ********************************************************************************** 
> IMPORTANT NOTE TO TA: The external lib named “pgl” has been created by ourselves.
> **********************************************************************************


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

# Credits

Date: April 2015

Author: Jonathan Link
