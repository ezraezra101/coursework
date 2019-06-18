#include "Frame.h"

// Create empty frame
df::Frame::Frame() {
	width = 0;
	height = 0;
	frame_str = "";
}

// Create frame of indicated width and height with string.
df::Frame::Frame(int new_width, int new_height, std::string new_frame_str) {
	width = new_width;
	height = new_height;
	frame_str = new_frame_str;
}

df::Frame::Frame(const Frame &other) {
	width = other.getWidth();
	height= other.getHeight();
	frame_str = other.getString();
}

// Getters and setters
void df::Frame::setWidth(int new_width) {
	width = new_width;
}
int df::Frame::getWidth(void) const {
	return width;
}
void df::Frame::setHeight(int new_height) {
	height = new_height;
}
int df::Frame::getHeight(void) const {
	return height;
}
void df::Frame::setString(std::string new_frame_str) {
	frame_str = new_frame_str;
}
std::string df::Frame::getString() const {
	return frame_str;
}