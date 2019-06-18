Homework #3
Graphics - CS 4731
Ezra Davis

Program description:
Draws a few L-systems in random colors.
Keys A-E draw L-systems, F draws several L-systems and a car.
The R key toggles rotation. (Which may be jerky. It takes a while to render an L-system.)

Building/Running:
The program should simply be setup to compile and run in visual studio.
You should also be able to compile it by hand by compiling main.cpp and PLY.cpp, which should require all the headers and libraries they need.


Code Organization:
* PLY handles PLY file loading and drawing. (In PLY.cpp)
* Grammar loads L-system files and does all the necessary string manipulation.
* Turtle draws the L-systems using a hierarchical stack (and Grammar's string)
* main.cpp handles glut and program state
* shaderUtils.cpp handles most of the interaction with the vertex shader.
* header.h ties all the headers and files together.

The ply files are in the GLSLExperiment/ply_files folder, the l-system files are in the lsys_files folder.

cube.cpp and plyHelper.cpp aren't actually used, but contains the code from the example for reference.

Each time the program moves to the next model, the ply file is reloaded.
