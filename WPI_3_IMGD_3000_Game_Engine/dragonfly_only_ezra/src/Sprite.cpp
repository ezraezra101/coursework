#include "Sprite.h"

// Private constructor
df::Sprite::Sprite() {
	// Don't call this!
}
// Destroy sprite, deleting any allocated frames.
df::Sprite::~Sprite() {
	if(frames != NULL) {
		delete [] frames;
	}
}

// Create sprite with indicated maximum number of frames
df::Sprite::Sprite(int max_frames) {
	frames = new df::Frame[max_frames];
	max_frame_count = max_frames;
	frame_count = 0;
	color = df::COLOR_DEFAULT;
	width = 0;
	height= 0;
	label = df::UNDEFINED_SPRITE;
}

// Getters and setters
void df::Sprite::setWidth(int new_width) {
	width = new_width;
}
int df::Sprite::getWidth(void) const {
	return width;
}
void df::Sprite::setHeight(int new_height) {
	height = new_height;
}
int df::Sprite::getHeight(void) const {
	return height;
}
void df::Sprite::setColor(df::Color new_color) {
	color = new_color;
}
df::Color df::Sprite::getColor() const {
	return color;
}
void df::Sprite::setLabel(std::string new_label) {
	label = new_label;
}
std::string df::Sprite::getLabel(void) {
	return label;
}

int df::Sprite::getFrameCount() const {
	return frame_count;
}

// Add frame to sprite.
// Return -1 if frame array is full, else 0.
int df::Sprite::addFrame(df::Frame new_frame) {
	if(frame_count == max_frame_count) {
		return -1; // Frames full
	} else {
		frames[frame_count] = new_frame;
		frame_count++;
		return 0;
	}
}

// Get next sprite frame indicated by number.
// Return empty frame if out of range [0, frame_count].
df::Frame df::Sprite::getFrame(int frame_number) {
	if(frame_number < 0 || frame_number >= frame_count) {
		return df::Frame();
	}

	return frames[frame_number];
}

