/*
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

*/

#include "..\\Header Files\\header.h"
#include "..\\Header Files\\PLY.h"

#include "..\\vendor\\bmpread.h"

//----------------------------------------------------------------------------

int width = 0;
int height = 0;

// handle to program
GLuint program;

bool shadows = true;
bool grass_floor = false;
bool reflection = false;
bool refraction = false;
bool rotating   = true;

float angle = 0;

static  GLuint  grass_texture = 0;
static  GLuint  stone_texture = 0;
static  GLuint  reflection_texture = 0;

void setMaterial(color4 diffuse, color4 specular, color4 ambient, GLfloat shininess, GLuint program);
void setLight(mat4 ctm,vec4 position, GLuint program);
//----------------------------------------------------------------------------

void initGPU() {

	// Load shaders
    program = makeProgram();
	// sets the default color to clear screen
    glClearColor( 0.0, 0.0, 0.0, 0.0 ); // black background

	setColor(color4(1,0,0,1), program);



	printf("Loading .bmps\n");
	bmpread_t grass, stones;

	if(!bmpread(".\\Image Files\\grass.bmp", 0, &grass))
	{
		fprintf(stderr, "%s:error loading bitmap file\n", ".\\Image Files\\grass.bmp");
		exit(1);
	}
	glActiveTexture( GL_TEXTURE0 );
	glGenTextures( 1, &grass_texture );
	glBindTexture( GL_TEXTURE_2D, grass_texture );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, grass.width, grass.height, 0, GL_RGB, GL_UNSIGNED_BYTE, grass.rgb_data );
	bmpread_free(&grass);

	if(!bmpread(".\\Image Files\\stones.bmp", 0, &stones))
	{
		fprintf(stderr, "%s:error loading bitmap file\n", ".\\Image Files\\stones.bmp");
		exit(1);
	}
	glActiveTexture( GL_TEXTURE1 );
	glGenTextures( 1, &stone_texture );
	glBindTexture( GL_TEXTURE_2D, stone_texture );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, stones.width, stones.height, 0, GL_RGB, GL_UNSIGNED_BYTE, stones.rgb_data );
	bmpread_free(&stones);
    
	

	//Loading Cubemap:
	GLuint cube_texture = 0;
//	glActiveTexture(GL_TEXTURE2);
//	glGenTextures(1, &cube_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_texture);

	glEnable(GL_TEXTURE_CUBE_MAP);
	bmpread_t cubeData;
	string cubeFileNames[] = {
		".\\Image Files\\env_map_sides\\nvposx.bmp",
		".\\Image Files\\env_map_sides\\nvposy.bmp",
		".\\Image Files\\env_map_sides\\nvposz.bmp",
		".\\Image Files\\env_map_sides\\nvnegx.bmp",
		".\\Image Files\\env_map_sides\\nvnegy.bmp",
		".\\Image Files\\env_map_sides\\nvnegz.bmp"
	};
	GLenum cubeSides[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	for(int i=0; i<6; i++) {
		if(!bmpread(cubeFileNames[i].c_str(), 0, &cubeData)) {
			fprintf(stderr, "%s:error loading bitmap file\n", cubeFileNames[i]);
			exit(1);
		}
		glTexImage2D(cubeSides[i], 0, GL_RGB, cubeData.width, cubeData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubeData.rgb_data);

		bmpread_free(&cubeData);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i( glGetUniformLocation(program, "cubeTex"), 2 );

    // Since our texture coordinates match our vertex positions, we
    //   can reuse the position data for our texture coordinates.
	// Not true for the cow, but we're not texturing that are we?
    GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" ); 
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    //Projection = glGetUniformLocation( program, "Projection" );

    // Set our texture samples to the active texture unit
    glUniform1i( glGetUniformLocation(program, "texture"), 1 );
    glBindTexture(GL_TEXTURE_2D, grass_texture);

	//Setting IOR for refraction
	glUniform1f( glGetUniformLocation(program, "IOR"), 0.7f);

	
}





//----------------------------------------------------------------------------
int t = 0;

// this is where the drawing should happen
void display( void )
{	
	static PLY cow(PLY::listFiles()[11], GL_FILL, true);
	static PLY floor(".\\ply_files\\1_square.ply", GL_FILL, true);

	static mat4 locations[] = {
		mat4(), //Cow
		RotateX(-90), //Floor
		Translate(0,0.5f,-0.5f)*RotateY(180),
		Translate(-0.5f,0.5f,0)*RotateY(90)
	};

	static color4 colors[] = {
		color4(1,1,1,1),
		color4(0.5f,0.5f,0.5f,1),
		color4(0.2f,0.2f,1,1),
		color4(0.2f,0.2f,1,1)
	};

	static int textures[] = {
		reflection_texture,
		grass_texture,
		stone_texture,
		stone_texture
	};

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 100.0);
	setPersp(perspectiveMat, program);

	//Constructing the CTM from its constituents. 
	Angel::mat4 modelMat =  LookAt(/*eye*/vec4(0.8f,1.5f,0.8f),/*at*/vec4(0,0.5f,0,1),vec4(0,1,0,1));


	//Setting up light
	vec4 light = Translate(0,0.8f,0)*RotateY(0.5f*angle) * vec4(0.01f,0,0,1); //Light location
	setLight(modelMat,light, program);
	static PLY lightSphere(".\\ply_files\\1_sphere.ply", GL_FILL, true); //Drawing sphere at light location...
	lightSphere.draw(modelMat * Translate(light.x,light.y,light.z)*Scale(0.1f), program);

	//Placing cow
	locations[0] = Translate(0,0.2,0) * RotateY(angle) * Scale(0.5);

	GLuint textured = glGetUniformLocationARB(program, "textured");
	glUniform1f(glGetUniformLocationARB(program, "texture_scale"), 2.0f);
	//PLY.draw
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);

	glUniform1i(textured, (reflection ? 2 : 1 )| (refraction ? 4 : 1));
	setColor(colors[0], program);
	cow.draw(modelMat*locations[0], program);

	if(shadows) {
		glUniform1i(textured,1);

		static mat4 to =  Translate(-light.x,-light.y,-light.z);
		static mat4 from= Translate(light.x+0.001f,light.y+0.001f,light.z+0.001f);
		static mat4 shadowMats[] = {
			Translate(-0.5f,0,-0.5) * //Not working :-(
			from * mat4(	1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							1/(-light.x+0.5f), 0, 0, 0) * to*Translate(0,0,0.5f),
			from * mat4(	1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 1/(-light.y), 0, 0) * to,
			Translate(0,0,0) * //Not working :-(
			from * mat4(	1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0,1/(-light.z), 0) * to*Translate(-0.0f,0,-0.0f),

		};

		
		
		setColor(color4(0,0,0,1), program);
		for(int i = 1; i<2; i++) { //The walls aren't working...
			cow.draw(modelMat*shadowMats[i]*locations[0],program);
		}
	}


	glUniform1i( textured, (int) grass_floor ); //No idea why I can't use 0...
	for(int i=1; i<4; i++) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		setColor(colors[i], program);
		floor.draw(modelMat * locations[i], program);
	}

    glFlush(); // force output to graphics hardware

	// use this call to double buffer
	glutSwapBuffers();
	// you can implement your own buffers with textures
}

//----------------------------------------------------------------------------

// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
	switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
	case 'a':
	case 'A':
		shadows = !shadows;
		break;
	case 'b':
	case 'B':
		grass_floor = !grass_floor;
		break;
	case 'c':
	case 'C':
		reflection = !reflection;
		break;
	case 'd':
	case 'D':
		refraction = !refraction;
		break;
	case 'r':
	case 'R':
		rotating = !rotating;
	default:
		return;
    }
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

// Reshape handler
void reshape(int x, int y) {
	//Placing viewport
	glViewport(0,0,x,y);

	width = x;
	height = y;
	glutPostRedisplay();
	
}
//----------------------------------------------------------------------------
#include <ctime>
void idle(void) {
	static clock_t time = clock();

	if(rotating && (clock() -time) > CLOCKS_PER_SEC/100) {
		time = clock();
		angle += 1.0f;
		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------
// entry point
int main( int argc, char **argv )
{
	// init glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
	width = 512;
	height = 512;

	glutInitContextVersion( 3, 3 ); // Why not? We can pretend we're using OpenGL 3.3

	// create window
	// opengl can be incorperated into other packages like wxwidgets, fltoolkit, etc.
    glutCreateWindow( "Ezra Davis HW 4" );

	// init glew
	glewExperimental = GL_TRUE; //Supposed to remove some errors - may not matter
    glewInit();

	initGPU();

	setMaterial(color4(.8f,.8f,.8f,1), color4(.8f,.8f,.8f,1), color4(0.2f,0.2f,0.2f,1), 50.0f, program);

	// assign handlers
    glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );
	glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}
