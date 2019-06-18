#ifndef __FRAME_H__
#define __FRAME_H__

#include <string>

namespace df {

class Frame {
private:
	int width;				// Width of frame
	int height;				// Height of frame
	std::string frame_str;	// All frame characters stored as an array
public:
	// Create empty frame
	Frame();

	// Create frame of indicated width and height with string.
	Frame(int new_width, int new_height, std::string frame_str);

	//Copy constructor
	Frame(const Frame &other);

	// Getters and setters
	void setWidth(int new_width);
	int getWidth(void) const;
	void setHeight(int new_height);
	int getHeight(void) const;
	void setString(std::string new_frame_str);
	std::string getString() const;

};

};

#endif
