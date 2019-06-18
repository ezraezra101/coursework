Homework #2
Graphics - CS 4731
Ezra Davis

Loads a series of PLY files and allows the user do simple manipulations by modifying a Current Transformation Matrix.

Building/Running:
The program should simply be setup to compile and run in visual studio.
You should also be able to compile it by hand by compiling main.cpp and PLY.cpp, which should require all the headers and libraries they need.


Code Organization:
All the PLY loading methods have been abstracted into a PLY class. (In PLY.cpp)
The other functions (and global variables) are in main.cpp.
header.h has the includes and prototypes.
All of the ply files are in the GLSLExperiment/ply_files folder.
cube.cpp isn't actually used, but contains the code from the example for reference.

Each time the program moves to the next model, the ply file is reloaded.
