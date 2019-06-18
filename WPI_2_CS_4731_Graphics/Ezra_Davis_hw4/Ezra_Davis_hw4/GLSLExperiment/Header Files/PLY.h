//Class for managing PLY files
#ifndef PLY_loaded
#define PLY_loaded


class PLY {
public:
	PLY(){ numPoints = 0; numPolys = 0; };
	PLY(const char *);
	PLY(const string);

	PLY(const char *, int fill);
	PLY(const string, int fill);

	PLY::PLY(const char * filename, int fill, bool normals);
	PLY::PLY(const string filename, int fill, bool normals);

	//Lists available files
	const static std::string *listFiles(void);

	void PLY::draw(mat4 ctm, GLuint program);

	color4 *getColors();

	//Gives a matrix that places the model at origin and rescales it so that it is within a (-1,-1,-1) to (1,1,1) cube.
	mat4 rescaleMatrix();

	
private:
	bool use_normals;
	bool fill;
	GLuint vao;
	GLuint buffer;//Vertex Array buffer
	int getVertsCount();
	size_t getVertsSize();

	float dimensions[6]; //Dimensions go in the order: x, y, z, -x, -y, -z
	void init(const char *);
	int numPoints;
	int numPolys;
	static color4 randomColor();

	static const string fileNames[];
};


//Other functions related to PLY files

	//Glut callbacks:
void displayPLY( void );
void keyboardPLY( unsigned char key, int x, int y );
void idlePLY( void );

#endif
