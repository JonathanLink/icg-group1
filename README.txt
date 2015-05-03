****************************
README
****************************

Description of the root directory items:

*include
   Contains all external libs headers
*lib
   Contains all external libs
*src
   Contains source of the project
*tex
   Contains all textures of the project


========================================================
IMPORTANT NOTE TO TA:
The external lib named “pgl” has been created by ourselves.
========================================================



How to build:
========================================================

0) Make sure to have OpenGL, GLEW, GLFW3 and PkgConfig.

Mac OSX install procedure (with brew):

GLEW:
brew install glew

GLFW3:
brew tap homebrew/versions
brew install glfw3

PkgConfig:
brew install pkg-config

1) cd myWorld
2) mkdir build
3) cd build
4) make ../ && make
5) To run the compiled executable: ./myWorld

(1 line to compile and run: cmake  ../ && make  && ./myWorld)

PGL Architecture:
========================================================

Description of all classes of the pgl library:

Window:
———————
It is a static class. For the moment, pgl can only handle one and unique window.

Scene:
——————
The window need to be associated to a Scene in order to display something.
A Scene is an abstract class which:
- will handle all keyboard/mouse input
- set a default FPS camera for the scene
- force its subclass to implement those 3 methods: init(), render(), cleanUp().

RenderObject:
————————————
Each object you want to render MUST be a subclass of RenderObject. That way our class is force to implement those 3 methods: init(), render(mat4 view, mat4 projection) and cleanUp().
IMPORTANT: In the init method of your subclass of Scene, don’t forget to call setScene(this);

Camera:
———————
Default camera of the pgl library. You never have to do anything with this class.
Scene object handle it for you.

FrameBuffer:
————————————
FrameBuffer object with bind and unbind functions. Draw your texture in between those 2 calls.

FOR A BETTER UNDERSTANDING, HAVE A LOOK AT THOSE SOURCE FILES: 
src/main.cpp, src/MyWorld.cpp, src/Terrain/Terrain.cpp

KNOWN BUGS:
———————————

- Camera weird behavior at the very beginning










******************************************************************************************

PoGL Background:
A small home-made library aiming to accelerate simple openGL projects or for prototyping purpose. PoGL actually stands for Prototyping OpenGL (name is subject to change, still brainstorming, fist name was pgl)

Date: April 2015
Author: Jonathan Link