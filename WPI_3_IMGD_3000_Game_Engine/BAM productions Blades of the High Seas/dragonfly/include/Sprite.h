#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <string>

#include "Frame.h"
#include "Color.h"

namespace df {

const std::string UNDEFINED_SPRITE = "df::undefined_sprite";

class Sprite {
private:
	int width;				// Sprite width.
	int height;				// Sprite height.
	int max_frame_count;	// Max number frames sprite can have.
	int frame_count;		// Current number of frames the sprite has.
	Color color;			// Optional color for entire sprite.
	Frame *frames;			// Array of frames.
	std::string label;		// Text label to identify sprite.
	Sprite();				// Private constructor.

public:
	// Destroy sprite, deleting any allocated frames.
	~Sprite();

	// Create sprite with indicated maximum number of frames
	Sprite(int max_frames);

	// Getters and setters
	void setWidth(int new_width);
	int getWidth(void) const;
	void setHeight(int new_height);
	int getHeight(void) const;

	void setColor(Color new_color);
	Color getColor() const;
	void setLabel(std::string new_label);
	std::string getLabel(void);

	int getFrameCount() const;

	// Add frame to sprite.
	// Return -1 if frame array is full, else 0.
	int addFrame(Frame new_frame);

	// Get next sprite frame indicated by number.
	// Return empty frame if out of range [0, frame_count].
	Frame getFrame(int frame_number);

};

};

#endif
