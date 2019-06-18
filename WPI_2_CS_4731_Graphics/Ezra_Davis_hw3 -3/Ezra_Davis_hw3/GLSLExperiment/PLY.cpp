

#include "header.h"
#include "PLY.h"
//PLY file management

const string PLY::fileNames[] = {
	".\\ply_files\\airplane.ply",
	".\\ply_files\\ant.ply",
	".\\ply_files\\apple.ply",
	".\\ply_files\\balance.ply",
	".\\ply_files\\beethoven.ply",
	".\\ply_files\\big_atc.ply",
	".\\ply_files\\big_dodge.ply",
	".\\ply_files\\big_porsche.ply",
	".\\ply_files\\big_spider.ply",
	".\\ply_files\\canstick.ply",
	".\\ply_files\\chopper.ply",
	".\\ply_files\\cow.ply",
	".\\ply_files\\dolphins.ply",
	".\\ply_files\\egret.ply",
	".\\ply_files\\f16.ply",
	".\\ply_files\\footbones.ply",
	".\\ply_files\\fracttree.ply",
	".\\ply_files\\galleon.ply",
	".\\ply_files\\hammerhead.ply",
	".\\ply_files\\helix.ply",
	".\\ply_files\\hind.ply",
	".\\ply_files\\kerolamp.ply",
	".\\ply_files\\ketchup.ply",
	".\\ply_files\\mug.ply",
	".\\ply_files\\part.ply",
	".\\ply_files\\pickup_big.ply",
	".\\ply_files\\pump.ply",
	".\\ply_files\\pumpa_tb.ply",
	".\\ply_files\\sandal.ply",
	".\\ply_files\\saratoga.ply",
	".\\ply_files\\scissors.ply",
	".\\ply_files\\shark.ply",
	".\\ply_files\\steeringweel.ply",
	".\\ply_files\\stratocaster.ply",
	".\\ply_files\\street_lamp.ply",
	".\\ply_files\\teapot.ply",
	".\\ply_files\\tennis_shoe.ply",
	".\\ply_files\\tommygun.ply",
	".\\ply_files\\trashcan.ply",
	".\\ply_files\\turbine.ply",
	".\\ply_files\\urn2.ply",
	".\\ply_files\\walkman.ply",
	".\\ply_files\\weathervane.ply"
};

PLY::PLY(const char *filename) {
	fill = false;
	init(filename);
}
PLY::PLY(const string filename) {
	fill = false;
	init(filename.c_str());
}

PLY::PLY(const char *filename, int filled) {
	fill = (filled == GL_FILL);
	init(filename);
}
PLY::PLY(const string filename, int filled) {
	fill = (filled == GL_FILL);
	init(filename.c_str());
}

static vec4 * loadFile(const char *filename, int &numPoints, int &numPolys, float *dimensions, int fill) {
	ifstream file;
	file.open(filename);
	if(!file.good()) {
		std::cout << "File not found";
		exit(1);
		return NULL;
	}

	string line;
	getline(file, line);
	if(line != "ply") {
		std::cout << "Not a PLY file!";
		exit(1);
		return NULL;
	}
	getline(file,line); //skip line

	//Read in headery stuff

	file >> line >> line >> numPoints;
	getline(file, line);//go to next line

	if(numPoints < 3 || numPoints > 100000) {
		cout << "Numpoints read from ply file is abnormal: " << numPoints << endl;
	}

	getline(file, line); getline(file, line); getline(file, line);  //Skip lines

	file >> line >> line >> numPolys;
	if(numPolys <=0 || numPolys > 100000) {
		cout <<  "Numpolys read from ply file is abnormal: " << numPolys << "\n" << endl;
		cout << line << endl << endl << endl;
	}
	getline(file, line);//go to next line

	getline(file, line);	getline(file, line);	//end header


	//Read in vertices
	vec4 *verts = new Angel::vec4[numPoints];
	for(int i=0; i<numPoints; i++) {
		file >> verts[i].x >> verts[i].y >> verts[i].z;
		verts[i].w = 1;

		if(verts[i].x > 20000 || verts[i].y > 20000 || verts[i].z > 20000) {
			std::cout << "Questionable vertex location";
		}



		//Getting dimensions:
		if(i==0) {
			dimensions[0] = verts[i].x;
			dimensions[1] = verts[i].y;
			dimensions[2] = verts[i].z;
			dimensions[3] = verts[i].x;
			dimensions[4] = verts[i].y;
			dimensions[5] = verts[i].z;
		}
		dimensions[0] = dimensions[0] > verts[i].x ? dimensions[0] : verts[i].x;
		dimensions[1] = dimensions[1] > verts[i].y ? dimensions[1] : verts[i].y;
		dimensions[2] = dimensions[2] > verts[i].z ? dimensions[2] : verts[i].z;
		dimensions[3] = dimensions[3] < verts[i].x ? dimensions[3] : verts[i].x;
		dimensions[4] = dimensions[4] < verts[i].y ? dimensions[4] : verts[i].y;
		dimensions[5] = dimensions[5] < verts[i].z ? dimensions[5] : verts[i].z;

	}

	//Read in polygons
	int three;
	int p1, p2, p3;

	int polyPoints = fill ? 3 : 6;

	vec4 *valuesList = new Angel::vec4[numPolys*polyPoints];

	for(int i=0; i<numPolys; i++) {
		file >> three >> p1 >> p2 >> p3;
		if(three != 3) {
			std::cout << "Error: wrong number of vertices in polygon. Attempting to continue" << std::endl;
		}
		if(p1 >= numPoints || p2 >= numPoints || p3 >= numPoints || p1 < 0 || p2 < 0 || p3 < 0) {
			std::cout << "Error: Invalid vertex index" << std::endl;
		}

		if(!fill) {
			valuesList[polyPoints*i]   = verts[p1];
			valuesList[polyPoints*i+1] = verts[p2];
			valuesList[polyPoints*i+2] = verts[p2];
			valuesList[polyPoints*i+3] = verts[p3];
			valuesList[polyPoints*i+4] = verts[p3];
			valuesList[polyPoints*i+5] = verts[p1];
		} else {
			valuesList[polyPoints*i]   = verts[p1];
			valuesList[polyPoints*i+1] = verts[p2];
			valuesList[polyPoints*i+2] = verts[p3];
		}
	}

	delete [] verts;


	return valuesList;
}
//Initializes a PLY file
//Abstracted from constructors
void PLY::init(const char *filename) {
	//Load file
	vec4 * valuesList = loadFile(filename, numPoints, numPolys, dimensions, fill);

	//Load PLY into GPU
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

	//Set up a VBO
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

	//Load arrays
	glBufferData(GL_ARRAY_BUFFER, getVertsSize(), valuesList, GL_STATIC_DRAW);

	delete [] valuesList;
}



//Draws the ply file
void PLY::draw(mat4 ctm, GLuint program) {
	int polyMode, drawMode;
	if(!fill) {
		polyMode = GL_LINES;
		drawMode = GL_LINES;
	} else {
		drawMode = GL_TRIANGLES;
		polyMode = GL_FILL;
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//Do PLY specific drawing settings and draw
	glPolygonMode( GL_FRONT_AND_BACK, polyMode );
	glEnable( GL_DEPTH_TEST );

	//Setup program
	glUseProgram(program);
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	setCTM(ctm * rescaleMatrix(), program);
	
	glDrawArrays(drawMode, 0, getVertsCount());

	glDisable( GL_DEPTH_TEST );
}










//Private methods:


// The number of vertices to be rendered (includes duplicates)
int PLY::getVertsCount() {
	int polyPoints = fill ? 3 : 6;
	return numPolys*polyPoints;//Sillyness for lines
}

//Size of the vertex array
size_t PLY::getVertsSize() {
	return sizeof(vec4)*getVertsCount();
}

//Function that returns one of 8 random colors
color4 PLY::randomColor() {
	static const color4 colors[] = {
		randColor(),
		randColor(),
		randColor(),
		randColor(),
		randColor(),
		randColor(),
		randColor(),
		randColor()
	};

	return colors[rand() % 8];
}

//Lists available PLY files
const string* PLY::listFiles() {
	return fileNames;
}

//Places model at origin and rescales it so that it is within the (-1,-1,-1) to (1,1,1) cube.
mat4 PLY::rescaleMatrix() {
	//Translating:
	mat4 translation = Angel::Translate(-(dimensions[0]+dimensions[3])/2,
										-(dimensions[1]+dimensions[4])/2,
										-(dimensions[2]+dimensions[5])/2);
	//Scaling:
	float ratio = 1/(dimensions[0]-dimensions[3]);
	float ratio2 = 1/(dimensions[1]-dimensions[4]);
	float ratio3 = 1/(dimensions[2]-dimensions[5]);
	ratio = ratio < ratio2 ? ratio : ratio2;
	ratio = ratio < ratio3 ? ratio : ratio3;
	mat4 scaling = Angel::Scale(ratio);

	return scaling * translation;
}