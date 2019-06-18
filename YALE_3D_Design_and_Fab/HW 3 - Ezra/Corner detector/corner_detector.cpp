#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#define BETWEEN(i,min,max) (((i) <= (max)) && (i) >= (min))
#define CLIP(i,min,max) ((i) > (max) ? (max) : ((i) < (min) ? (min) : (i)) )
#define IDX(i, j) (CLIP(i, 0, width)*width + CLIP(j,0, height)) // Dumbly using nearest item when over the edge.
// Perform all calculations with this:
#define INT_T int

#define MAKE_TEMP_BUFFER() ((INT_T *)alloca(sizeof(INT_T)*width * height));

#define ASCII_OUT false

unsigned char *pic;
int width, height;

// Reads a ppm into the *pic global variable, and also sets width and height.
void readPPM(std::string filename) {
	std::ifstream file(filename);
	if(!file.good()) {
		std::cout << "Invalid file " << filename << std::endl;
		exit(1);
	}
 	
 	std::string line;
 	file >> line;
 	if(line[0] != 'P' || line[1] != '6') {
 		std::cerr << "Not a valid ppm file" << std::endl;
 		exit(1);
 	}
 	// Ignoring comments...
 	if(file.peek() == '#') {
 		std::cerr << "This program is lazy and cannot handle comments in ppm files." << std::endl;
 		exit(1);
 	}

 	int res;
 	file >> width;
 	file >> height;
 	file >> res;

 	if(res != 255) {
 		std::cerr << "Not a 8-bit color depth image" << std::endl;
 		exit(1);
 	}

 	pic = (unsigned char *) malloc(sizeof(INT_T)*3 * width * height);

 	file.read((char *)pic, 1); //removing dumb newline
 	file.read((char *)pic, 3*width*height*sizeof(char)); // Copy rgb into the buffer


 	file.close();
}

// Copy rgb image from pic into out
void greyscale(INT_T *out, unsigned char *pic, int width, int height) {
 	INT_T r, g, b;
 	for(int i=0; i<width*height; i++) {
 		r = pic[3*i+0]; g = pic[3*i+1]; b = pic[3*i+2];
 		out[i] = 0.3*r+0.5*g+0.2*b;
 	}
}

// Out, a and b can all be the same variable without problems.
void pointwiseMultiply(INT_T *out, INT_T *a, INT_T *b, int length) {
	for(int i=0; i<length; i++) {
		out[i] = a[i]*b[i];
	}
}

// Convolves `filter` (a 1x`filterWidth` array) over `in` (a `width`x`height` array)
void convolve(INT_T *out, INT_T *in, INT_T *filter, bool vertical, int filterWidth, int width, int height) {

	for(int idxInFilter=0; idxInFilter < filterWidth; idxInFilter++) {
		for(int i=0; i<width; i++) {
			for(int j=0; j<height; j++) {

				if(idxInFilter == 0) {
					out[IDX(i,j)] = 0;
				}

				if(vertical) {
					if(BETWEEN(j+idxInFilter-(filterWidth/2), 0, height)) {
						out[IDX(i,j)] += filter[idxInFilter] * in[IDX(i,j+idxInFilter-(filterWidth/2))];
					}
				} else {
					if(BETWEEN(i+idxInFilter-(filterWidth/2), 0, width)) {
						out[IDX(i,j)] += filter[idxInFilter] * in[IDX(i+idxInFilter-(filterWidth/2),j)];
					}
				}

			} // inner for pixel & filter index loop
		}
	}
}

void normalize(INT_T *out, INT_T *in, int min, int max, int length) {
	INT_T bufMax = 0;
	INT_T bufMin = 10000;
	for(int i=0; i<length; i++) {
		if(in[i] > bufMax) {
			bufMax = in[i];
		}
		if(in[i] < bufMin) {
			bufMin = in[i];
		}
	}
	for(int i=0; i<length; i++) {
		out[i] = ((max-min)*(in[i]-bufMin))/(float) ((bufMax-bufMin)) + min;
	}
}

void cornerDetect(INT_T *out, INT_T *buf_x, INT_T *buf_y, INT_T *buf_xy, int length) {
	for(int i=0; i<length; i++) {
		INT_T trace = buf_x[i] + buf_y[i];
		INT_T determinant = buf_x[i] * buf_y[i] - buf_xy[i]*buf_xy[i];
		out[i] = determinant - (0.04 * trace*trace);
	}
}


void writePGM(INT_T *buf, int width, int height) {

	std::ostream &out = std::cout;

	out << "P5\n";
	out << width << " " << height << " 255\n";

	for(int i=0; i<width*height; i++) {
		if(ASCII_OUT)
			out << (int) buf[i] << " ";
		else
			out << (unsigned char) buf[i];
	}
}

// Check for incorrect usage, and correct it if not.
void usage_message(int argc, char **argv) {
  if(argc != 2) {
    std::cerr << "Usage:\n\t" << argv[0] << " <filename> >out.pgm" << std::endl;
    exit(1);
  }
}

int main(int argc, char **argv) {
	usage_message(argc, argv);

	readPPM(argv[1]);
	INT_T *greyPic = MAKE_TEMP_BUFFER();
	greyscale(greyPic, pic, width, height);
	free(pic);

	// Find directional derivatives:
	INT_T *buf_x = MAKE_TEMP_BUFFER();
	INT_T *buf_y = MAKE_TEMP_BUFFER();
	INT_T filter[] = {-2, -1, 0, 1, 2};
	convolve(buf_x, greyPic, filter,false, 5, width, height);
	convolve(buf_y, greyPic, filter,true, 5, width, height);


	INT_T *buf_xy = MAKE_TEMP_BUFFER();
	// Calculate the I matrix for each point (xx, xy; xy yy)

	pointwiseMultiply(buf_y, buf_x, buf_y, width*height);
	pointwiseMultiply(buf_x, buf_x, buf_x, width*height);
	pointwiseMultiply(buf_y, buf_y, buf_y, width*height);
	// values now range from 0 to 65025, and may be negative for xy.
	// Incidently, INT_T should have more than 16 bits...
	normalize(buf_x, buf_x, -1024,1023, width*height);
	normalize(buf_y, buf_y, -1024,1023, width*height);
	normalize(buf_xy, buf_xy, -1024,1023, width*height);
	// Now 11 bits are used

	// Now average it over a window
	// Incidently... it turns out it is really important to do this after the previous step.
	INT_T *buf_swap = MAKE_TEMP_BUFFER();
	INT_T filter_2[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	convolve(buf_swap, buf_x, filter_2, false, 15, width, height);
	convolve(buf_x, buf_swap, filter_2, false, 15,  width, height);

	convolve(buf_swap, buf_y, filter_2, 15, false, width, height);
	convolve(buf_y, buf_swap, filter_2, 15, true, width, height);

	convolve(buf_swap, buf_xy, filter_2, 15, false, width, height);
	convolve(buf_xy, buf_swap, filter_2, 15, true, width, height);

	// Now I should normalize again... bah... it does come really close to my 32 bit soft limit...
	normalize(buf_x, buf_x, -1024,1023, width*height);
	normalize(buf_y, buf_y, -1024,1023, width*height);
	normalize(buf_xy, buf_xy, -1024,1023, width*height);
	// Now 11 bits are used

	// This is the part that is currently causing trouble...
	INT_T *buf_out = MAKE_TEMP_BUFFER();
	cornerDetect(buf_out, buf_x, buf_y, buf_xy, width*height);

	INT_T *normalized = MAKE_TEMP_BUFFER();
	normalize(normalized, buf_out, 0,255, width*height);


	writePGM(normalized, width, height);
}