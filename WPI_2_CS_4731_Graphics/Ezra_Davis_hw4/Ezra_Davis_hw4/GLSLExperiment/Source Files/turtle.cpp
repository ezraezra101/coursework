#include "..\\Header Files\\header.h"

GLuint Turtle::program = 0;
GLuint Turtle::buffer  = 0;

#define SPHERE_RADIUS 0.5

#include <stack>
struct LocationStack {
	stack<mat4> ctms;
	stack<vec4> directions;

	LocationStack() {
	}
	void push(mat4 ctm, vec4 direction) {
		ctms.push(ctm);
		directions.push(direction);
	}
	void pop(mat4& ctm, vec4& direction) {
		ctm       = ctms.top();
		direction = directions.top();
		ctms.pop();
		directions.pop();
	}
};





Turtle::Turtle(const char * filename) {
  grammar = Grammar(filename);
  pattern = grammar.getPattern();//Kinda silly
//  assemble();
}


void Turtle::assemble(mat4 ctm, GLuint program) {

	vec4 rotation= normalize(vec4(0,1,0,0));

	LocationStack stack;

	drawSphere(ctm, vec4(0,0,0,0), SPHERE_RADIUS,program);

	int index=0;
	for(unsigned int i=0; i<pattern.length(); i++) {
		switch (pattern[i]) {
		
		//Moving
		case 'F':
			drawPolyCylinder(ctm,vec4(0,0,0,1),vec4(0,1,0,1),program);
			ctm = ctm * Translate(0,grammar.length(),0);
			//ctm = ctm * Translate(rotation *(GLfloat)grammar.length());
			break;
		case 'f':
			ctm = ctm * Translate(0,grammar.length(),0);
			//ctm =  ctm * Translate(rotation *(GLfloat)grammar.length());
			break;

		//X rotation
		case '+':
			ctm = ctm * Translate(ctm[3][0],ctm[3][1],ctm[3][2]) * RotateX(grammar.rotationX()) * Translate(-ctm[3][0],-ctm[3][1],-ctm[3][2]);
			//rotation = normalize(Angel::RotateX(grammar.rotationX()) * rotation);
			//rotation.x += grammar.rotationX();
			break;
		case '-':
			ctm = ctm * Translate(ctm[3][0],ctm[3][1],ctm[3][2]) * RotateX(-grammar.rotationX()) * Translate(-ctm[3][0],-ctm[3][1],-ctm[3][2]);
			//rotation = normalize(Angel::RotateX(-grammar.rotationX()) * rotation);
			break;

		//Y rotation
		case '&':
			ctm = ctm * Translate(ctm[3][0],ctm[3][1],ctm[3][2]) * RotateY(grammar.rotationY()) * Translate(-ctm[3][0],-ctm[3][1],-ctm[3][2]);
			//rotation = normalize(Angel::RotateY(grammar.rotationY()) * rotation);
			break;
		case '^':
			ctm = ctm * Translate(ctm[3][0],ctm[3][1],ctm[3][2]) * RotateY(-grammar.rotationY()) * Translate(-ctm[3][0],-ctm[3][1],-ctm[3][2]);
			//rotation = normalize(Angel::RotateY(-grammar.rotationY()) * rotation);
			break;

		//Z rotation
		case '\\':
			ctm = ctm * Translate(ctm[3][0],ctm[3][1],ctm[3][2]) * RotateZ(grammar.rotationZ()) * Translate(-ctm[3][0],-ctm[3][1],-ctm[3][2]);
			//rotation = normalize(Angel::RotateZ(grammar.rotationZ()) * rotation);
			break;
		case '/':
			ctm = ctm * Translate(ctm[3][0],ctm[3][1],ctm[3][2]) * RotateZ(-grammar.rotationZ()) * Translate(-ctm[3][0],-ctm[3][1],-ctm[3][2]);
			//rotation = normalize(Angel::RotateZ(-grammar.rotationZ()) * rotation);
			break;

		//other items
		case '|'://U turn
			rotation.x = -rotation.x;
			rotation.y = -rotation.y;
			rotation.z = -rotation.z;
			break;

		case '[':
			stack.push(ctm,rotation);
			break;
		case ']':
			stack.pop(ctm,rotation);
			break;
		case '\n':
			break;
		case ' ':
			break;
		default:
			cout << "Invalid Pattern:" << pattern[i] << " (" << (int) pattern[i] << ")" << endl;

		}
	}
}


void Turtle::draw(mat4 ctm, GLuint program) {
	GLint oldVBO;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &oldVBO);

	assemble(ctm, program);

	glBindBuffer(GL_ARRAY_BUFFER, oldVBO);
}

void Turtle::drawSphere(mat4 ctm, point4 p, float size, GLuint program) {
	static PLY sphere(".\\ply_files\\1_sphere.ply");
	sphere.draw(ctm *Translate(p.x, p.y,p.z) * Scale(size), program);
}


void Turtle::drawCylinder(mat4 ctm, point4 start, point4 end, float size, GLuint program) {
	const float PI = 3.1415926535897932384626433f;
	
	start.w = 0;end.w = 0; //for normalizing
	point4 angle = normalize(end - start);

	//Assemble matrices
	float phi = asin(angle.y);//angle above x axis
	float theta = atan(angle.z/angle.x); //Angle around Y axis

	phi = angle.x > 0 ? phi : PI-phi;
	
	if(angle.y >= 0.999999f) {
		//phi = PI/2;
		theta = 0;
	} else if(angle.y <= -0.999999f) {
		//phi = -PI/2;
		theta = 0;
	}

	mat4 rotationMatrix = RotateY(theta*180/PI) * RotateZ(phi*180/PI-90) * RotateX(90);

	vec4 avg = (start + end)/2;
	mat4 translationMatrix = Translate(avg.x,avg.y,avg.z);
	mat4 scaleMatrix = Scale(SPHERE_RADIUS,SPHERE_RADIUS,1.0*0.705f);

	//draw!
	static PLY cylinder(".\\ply_files\\1_cylinder.ply");
	cylinder.draw(ctm * translationMatrix * rotationMatrix * scaleMatrix, program);
	return;
}

void Turtle::drawPolyCylinder(mat4 ctm, point4 start, point4 end, GLuint program) {
	point4 avg = (start + end)/2;
	float length = Angel::length(start-end);

	//	setColor(point4(0,1,0,1),program);
	drawCylinder(ctm, start, avg,length,program);
	//	setColor(point4(1,0,0,1),program);
	drawSphere(ctm, avg, SPHERE_RADIUS, program);
	//	setColor(point4(0,1,0,1),program);
	drawCylinder(ctm, avg, end,length,program);
	//	setColor(point4(1,0,0,1),program);
	drawSphere(ctm, end, SPHERE_RADIUS, program);
}