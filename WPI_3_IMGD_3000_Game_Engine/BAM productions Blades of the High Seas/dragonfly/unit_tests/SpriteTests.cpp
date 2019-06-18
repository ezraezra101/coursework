#include <stdio.h>
#include <string>
#include "Sprite.h"

int main() {

	int failed_tests = 0;

	df::Frame f = df::Frame(1,1, "X"), f_default = df::Frame();

	df::Sprite s = df::Sprite(1); // A sprite with up to one frame.

	if(s.getWidth() != 0 || s.getHeight() != 0 || 
		s.getColor() != df::COLOR_DEFAULT || s.getLabel() != df::UNDEFINED_SPRITE ||
		s.getFrameCount() != 0 || s.getFrame(1).getHeight() != 0) { // s.getFrame should return an 'empty' frame.
		printf("Sprite's constructor doesn't set defaults correctly!\n");
		failed_tests++;
	}

	s.setWidth(1);
	s.setHeight(2);
	s.setColor(df::BLUE);
	s.setLabel("test");
	s.addFrame(f); // Note: doesn't force frame to fit widthxheight

	if(s.getWidth() != 1 || s.getHeight() != 2 || 
		s.getColor() != df::BLUE || s.getLabel() != "test") {
		printf("Sprite's getters and setters don't work!\n");
		failed_tests++;
	}
	if(s.getFrame(0).getString() != f.getString() || s.getFrameCount() != 1) {
		printf("Unable to add frames!");
		failed_tests++;
	}

	if(-1 != s.addFrame(f)) {
		printf("Sprite can load more than max_frame_count frames.\n");
		failed_tests++;
	}

	return failed_tests;
}