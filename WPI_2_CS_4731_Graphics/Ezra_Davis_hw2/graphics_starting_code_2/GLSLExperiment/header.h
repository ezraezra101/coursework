#include "Angel.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

// Prototypes:
void initGPU( void );
void initPLY(int fileNum); //Loads and prepares a PLY file
void drawPLY();
	//Glut callbacks:
void display( void );
void keyboard( unsigned char key, int x, int y );
void reshape(int width, int height);
void idle( void );


//Class for managing PLY files
class PLY {
public:
	PLY(){ numPoints = 0; numPolys = 0; };
	PLY(const char *);
	PLY(const string);
	PLY::PLY(const PLY &that);
	~PLY(); //Destructor
	void PLY::operator=(const PLY& that);		//Adhering to the rule of three == important

	//Lists available files
	const static std::string *listFiles(void);

	int getVertsCount();
	size_t getVertsSize();
	vec4 *getVerts();

	color4 *getColors();

	//Gives a matrix that places the model at origin and rescales it so that it is within a (-1,-1,-1) to (1,1,1) cube.
	mat4 rescaleMatrix();

	
private:
	float dimensions[6]; //Dimensions go in the order: x, y, z, -x, -y, -z
	void init(const char *);
	int numPoints;
	int numPolys;
	vec4 *valuesList;
	vec4 *colorsList;
	static color4 randomColor();

	static const string fileNames[];
};
