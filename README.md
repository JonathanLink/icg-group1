README
==================================

You can have a look at the repo and the README online on GitHub:
* https://github.com/JonathanLink/icg-group1/blob/master/README.md

You can also access the presentation we did (with the TAs):
* https://docs.google.com/presentation/d/1smA_cXriQBdCqXfAcJgoeClScihTdeLJtaJhzFbnZtE/pub?start=false&loop=false&delayms=3000

# Group 1

Members
* Adan Häfliger
* Jonathan Link
* Pierre Walch

# Description

We implemented all the mandatory features, and added many extensions:
* General
    * We have our own code base. We wrote our own OpenGL library (POGL). You can read more about this at the bottom of the README.
* Part 1
    * multiple terrains (island, moutains, live editor from the program(!))
* Part 2
    * water reflection
    * water dynamics
    * water depth (camera under water)
    * underwater camera
    * fog
    * the sun moves around the terrain with a parabola
* Part 3
    * mouse for view direction
    * jumping
    * bezier curve (with speed control)
    * advanced edit bezier mode
    * rain particles

Other small features:
   * wireframe mode
   * switch between 3 Bezier cameras
       * bird-eye view
       * lake view
       * parabolic view

# Keyboard tips:

Press:

* C to change camera (FLY, FPS, BEZIER)
* W,A,S,D + mouse in FLY and FPS mode	
* SPACE BAR to jump in FPS mode
* R to turn on/off the rain
* F to turn on/off the fog
* L to turn on/off wireframe mode
* V to switch between Bezier curves
* J to decrease camera bezier speed
* K to increase camera bezier speed

# Bezier Mode:

Press:

* B to enter Bezier mode
* U to select the first handle
* I to select the second handle
* O to select the third handle
* P to select the forth and last handle
* X to move along the X-axe
* Y to move along the Y-axe
* Z to move along the Z-axe
* UP ARROW to increase value
* DOWN ARROW to decrease value
* RIGHT ARROW to increase step value
* LEFT ARROW to decrease step value
* ENTER to output the 4 handles coordinates into the console

# Perlin Mode:

Press:

* P to enter Bezier mode
* SPACE BAR to select which perlin parameter you want to modify
* UP ARROW to increase value
* DOWN ARROW to decrease value
* O to write all bezier parameters into the console
* R to reset the terrain to default bezier parameters

And finally: press CTR-ALT-F3-P-MAJ to ... no just kidding!


# Code:
Description of the root directory items:

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

# PoGL Architecture:

Description of all classes of the PoGL library:

* Window:
    * PoGL can only handle one and unique instance of window. Window is therefore a singleton.

* Scene
    * The window need to be associated to a Scene in order to display something.
    * A Scene is an abstract class which:
        * will handle all keyboard/mouse input
        * set a default FLY/FPS camera for the scene
        * force its subclass to implement those 3 methods: init(), render(), cleanUp().
    * Setup common uniform variables like model, view, projection, modelView, lightPosition, cameraPosition, ...
    * The scene object will be use to display all your subclasses of RenderObject.

* RenderObject
    * Each object you want to render MUST be a subclass of RenderObject. That way our class is force to implement those 3 methods: init(), render(mat4 view, mat4 projection) and cleanUp().
    * IMPORTANT: In the init method of your subclass of Scene, don’t forget to call yourRenderObject.setScene(this);
    RenderObject offers a method to load your vertex and fragments shader and one method to generate a 2D texture.
    
* Camera:
    * Default camera of the PoGL library. You never have to do anything with this class. 
    * Scene object handle it for you.

* FrameBuffer
    * FrameBuffer object with bind and unbind functions. Draw your texture in between those 2 calls.

* Skybox
    * Manage a simple skybox

# PoGL Background

A small home-made library aiming to accelerate simple OpenGL projects or for prototyping purpose. PoGL actually stands for Prototyping OpenGL (name is subject to change, still brainstorming, fist name was pgl). It allows you to quickly set up a multiplatform openGL application. It handles for you 2 types of camera (FLY and BEZIER). PoGL is still in development phase.
