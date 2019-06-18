Homework #4
Graphics - CS 4731
Ezra Davis

Program description:
A simple scene that does reflection, refraction, and shadows
The A key toggles shadows.
The B key toggles textures.
The C key toggles reflection.
The D key toggles refraction. (can be stacked with reflection)
The R key toggles rotation.

Building/Running:
The program should simply be setup to compile and run in visual studio.
You should also be able to compile it by hand by compiling main.cpp and PLY.cpp, which should require all the headers and libraries they need.


Code Organization:
Folder organization:
	Shader files have my vertex shader & fragment shader.
	Source files contains all the .cpp files I've written.
	Header Files contains the headers I've written.
	Image Files, ply_files, and lsys_files contain files that could be loaded into the program (only a few of the ply files are used, and none of the l_systems).
	vendor contains all 3rd party files.

* PLY handles PLY file loading and drawing. (In PLY.cpp)
* main.cpp handles glut and program state
* shaderUtils.cpp handles most of the interaction with the vertex shader.
* header.h ties all the headers and files together.

Legacy files:

* Grammar loads L-system files and does all the necessary string manipulation.
* Turtle draws the L-systems using a hierarchical stack (and Grammar's string)
cube.cpp and plyHelper.cpp aren't actually used, but contains the code from the example for reference.

The ply files are in the GLSLExperiment/ply_files folder, the l-system files are in the lsys_files folder.