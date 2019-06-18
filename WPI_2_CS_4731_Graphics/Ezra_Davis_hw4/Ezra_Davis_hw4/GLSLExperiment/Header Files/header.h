#include "..\\vendor\\Angel.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

// Prototypes:
void initGPU( void );
	//Glut callbacks:
void display( void );
void keyboard( unsigned char key, int x, int y );
void reshape(int width, int height);
void idle( void );

void setPersp(Angel::mat4 perspectiveMat, GLuint program);
void setCTM(Angel::mat4 modelMat, GLuint program);
void setColor(color4 color, GLuint program);
GLuint makeProgram();

#include "..\\Header Files\\grammar.h"
#include "..\\Header Files\\PLY.h"
#include "..\\Header Files\\turtle.h"

//Helper function for picking the 8 random colors
inline color4 randColor() {
		return color4(rand()/ (float) RAND_MAX,
		rand()/ (float) RAND_MAX,
		rand()/ (float) RAND_MAX,
		1.0);
}