#include "header.h"
#include "PLY.h"
//Contains helper functions for PLY.cpp
//Mostly just copied and pasted from assignment #2

int fileNum = 0; //The active file's number
const int MaxFileNum = 43; //Number of PLY files
PLY activeFile;
int angular_velocity = 0;
int velocity[3] = {0,0,0};

extern float degrees;
extern float degreesStart;

float degrees = 0;
float degreesStart = 0;

extern mat4 rotation;
extern mat4 translation;
//extern mat4 shear;
extern GLuint program;


//----------------------------------------------------------------------------

// keyboard handler
void keyboardPLY( unsigned char key, int x, int y )
{
	static int colors = 1;
	static GLfloat s = 0; //Shear amount
	GLuint colored;
	static int twist = 0;

	switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
	case 'w':
	case 'W':
		//Draw wireframe at a suitable initial position from the viewer.
		translation = Angel::identity();
		velocity[0] = 0; velocity[1] = 0; velocity[2] = 0;
		angular_velocity = 0;
		degrees = 0;
		degreesStart = 0;
		rotation = Angel::identity();
		break;
	case 'n':
	case 'N':
		fileNum = (fileNum+1) % MaxFileNum;
		cout << PLY::listFiles() [fileNum] << endl;
//		initPLY(fileNum);
		break;
	case 'p':
	case 'P':
		fileNum = fileNum == 0 ? MaxFileNum-1 : fileNum-1;
		cout << PLY::listFiles() [fileNum] << endl;
//		initPLY(fileNum);
		break;
	case 'x': //Toggle -ve X direction
		velocity[0] = velocity[0] == -1 ? 0 : -1;
		break;
	case 'X': //Toggle +ve X direction
		velocity[0] = velocity[0] == 1 ? 0 : 1;
		break;
	case 'y': //Toggle -ve Y direction
		velocity[1] = velocity[1] == -1 ? 0 : -1;
		break;
	case 'Y': //Toggle +ve Y direction
		velocity[1] = velocity[1] == 1 ? 0 : 1;
		break;
	case 'z': //Toggle -ve Z direction
		velocity[2] = velocity[2] == -1 ? 0 : -1;
		break;
	case 'Z': //Toggle +ve Z direction
		velocity[2] = velocity[2] == 1 ? 0 : 1;
		break;

	case 'r':
	case 'R': //Rotate wireframe around current position & do a slideshow
		angular_velocity = angular_velocity == 0 ? -1 : 0;
		degreesStart = (degrees+ degreesStart);
		degrees = 0;
		fileNum = angular_velocity == 0 ? fileNum : fileNum-1;
		break;
	case 'c':
	case 'C': //Toggle between red color & many colors
		colors = colors == 1 ? 0 : 1;
		colored = glGetUniformLocation(program, "colored");
		glUniform1i( colored, colors );
		break;
	case 'h': //Increase X shearing (read instructions)
		s+= (GLfloat) 0.4f;
	case 'H': //Decrease X shearing
		/*
		s = s >= 0.2f ? s -0.2f : 0;
		shear = Angel::mat4(1,s,0,0,
							0,1,0,0,
							0,0,1,0,
							0,0,0,1);
							*/
		break;
	case 't': //Increase Y twisting (read instructions)
		twist += 2;
	case 'T': //Decrease Y twisting.
		twist = twist > 0 ? twist-1 : twist;
		//twisted = glGetUniformLocation(program, "twisted");
		//glUniform1f( twisted, (GLfloat) twist*0.02f );
		break;
	default:
		return;
    }
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
// Idle handler
void idlePLY() {
	if(angular_velocity) {
		degrees += angular_velocity/10.0f;
		cout << degrees << endl;
		if(degrees > 360 || degrees < 0.0f) {
			degrees =		   degrees > 0.0f ? 360.0f : 0.0f;
			angular_velocity = degrees > 0 ? -1  : 1;
			fileNum = (fileNum+1) % MaxFileNum;
			cout << PLY::listFiles() [fileNum] << endl;
//			initPLY(fileNum);
		}
	}

	//Figure out rotation:
	const GLfloat degreesToRadians = 3.1415926535897932384626433f/180;
	rotation    = Angel::RotateY(degrees+degreesStart);

	//Figure out translation:
	const GLfloat speedFactor = 0.0005f;
	translation = translation  + 
		Angel::mat4(0,0,0,speedFactor * velocity[0],
					0,0,0,speedFactor * velocity[1],
					0,0,0,speedFactor * velocity[2],
					0,0,0,0);



	glutPostRedisplay();
}



//----------------------------------------------------------------------------




//----------------------------------------------------------------------------




//----------------------------------------------------------------------------