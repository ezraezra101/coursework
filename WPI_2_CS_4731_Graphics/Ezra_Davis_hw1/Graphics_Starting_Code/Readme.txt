Completed homework 1 for CS 4731 (Graphics)
 Written by Ezra Davis 9/12/2014
 Draws a variety of items, a Sierpinski triangle, Hilbert's space filling curve, and a few polyline files.

 Files are located in the Graphics_Starting_Code\GLSL\ directory

 Files and contents:
	Angel.h		-	general purpose header file (untouched)
	CheckError.h -	OpenGL error reporting		(untouched)
	dragon.dat	-	Dragon polyline file		(untouched)
*	example1.cpp -	Main file of the program.
					Contains main method, setup functions,
					glut callbacks, and global variables.
					Also includes the rest of the required files.
	freeglut.dll -	Glut library				(untouched)
*	fshader1.glsl -	Primary fragment shader
*	fshader2.glsl - Fragment shader for Hilbert curve's background
	glew32.dll	  - Glew library				(untouched)
*	hilbert.cpp	  - Contains functions related to Hilbert's space filling curve.
	InitShader.cpp- Loads and compiles GLSL files (untouched)
	mat.h		-	Matrix related header		(untouched)
	mat_old_from_angel.h - I assume it does the same thing as mat.h (untouched)
*	polyLines.cpp - Contains functions related to polyline file drawing.
*	sierpinski.cpp- Contains functions related to sierpinski triangle.
	textfile.cpp  - Loads files into C-Strings	(untouched)
	usa.dat		  - USA polyline file			(untouched)
	vec.h		  - Vector related header		(untouched)
	vinci.dat	  - Vinci polyline file			(untouched)
*	vshader1.glsl - Primary vertex shader		(barely modified)
*	vshader2.glsl - Vertex shader for Hilbert curve's background

	Note: there are no modules or encapsulation. All shared variables are simple defined as global variables in example1.cpp

 Compiling & running:
	The program should be set up for visual studio to compile and run by opening Graphics_Starting_Code\GLSLExperiment.sln and pressing Ctrl-F5.

	Other .cpp files are intentionally not included in the visual studio project. they are included manually in example1.cpp
	Simply making sure that all the above files are in the same directory and compiling example1.cpp should compile the whole program.

