#ifndef __GRAPHICS_MANAGER_H__
#define __GRAPHICS_MANAGER_H__

// Library includes
#include <SFML/Graphics.hpp>

// Engine includes
#include "Manager.h"
#include "Color.h"
#include "Frame.h"
#include "Position.h"

namespace df {

enum Justification {
	LEFT_JUSTIFIED,
	CENTER_JUSTIFIED,
	RIGHT_JUSTIFIED
};


// Defaults for SFML window.
const int WINDOW_HORIZONTAL_PIXELS_DEFAULT = 1024;
const int WINDOW_VERTICAL_PIXELS_DEFAULT = 768;
const int WINDOW_HORIZONTAL_CHARS_DEFAULT = 160;
const int WINDOW_VERTICAL_CHARS_DEFAULT = 48;
const int WINDOW_STYLE_DEFAULT = sf::Style::Titlebar;
const df::Color WINDOW_BACKGROUND_COLOR_DEFAULT = df::BLACK;
const std::string WINDOW_TITLE_DEFAULT = "Dragonfly";
const std::string FONT_FILE_DEFAULT = "df-font.ttf";

// These numbers are used for drawCh();
const int MAGIC_RECTANGLE_OFFSET_X = 6/(WINDOW_HORIZONTAL_CHARS_DEFAULT/80);
const int MAGIC_RECTANGLE_OFFSET_Y = 12/(WINDOW_VERTICAL_CHARS_DEFAULT/24);

class GraphicsManager : public Manager {
	sf::Font font;	// Font used for ASCII graphics
	sf::RenderWindow *p_window; // Pointer to SFML window
	int window_horizontal_pixels;
	int window_vertical_pixels;
	int window_horizontal_chars;
	int window_vertical_chars;
	df::Color window_background_color;

	// It's a singleton class, so don't make these public.
	GraphicsManager();
	GraphicsManager(GraphicsManager const &);
	void operator=(GraphicsManager const &);

	// Compute character height, based on window size and font.
	float charHeight() const;

	// Compute character width, based on window size and font.
	float charWidth() const;
	// Convert ASCII space (x,y) to window pixels (x,y).
	df::Position spacesToPixels(df::Position spaces) const;

	// Convert window pixels (x,y) to ASCII spaces (x,y).
	df::Position pixelsToSpaces(df::Position pixels) const;

public:
	~GraphicsManager();

	// Get the ONE instance of GraphicsManager
	static GraphicsManager &getInstance();

	// Start up graphics window ready for text-based display
	// Return 0 (ok) or -1 (fail)
	int startUp();

	// Close graphics window
	void shutDown();

	// Draw character at screen location (x,y) with color.
	// Return 0 if ok, -1 if not.
	int drawCh(Position world_pos, char ch, Color color) const;

	// Draw string at window location (x,y)
	// Justified left, center, or right.
	// Return 0 if OK, else -1.
	int drawString(Position pos, std::string str, Justification just, Color color) const;

	// Draw single sprite frame aty window location (x, y) with color.
	// If centered true then center frame at (x, y).
	// Return 0 if ok, else -1
	int drawFrame(Position world_pos,
		Frame frame, bool centered, Color color) const;

	// Sets the window's background color
	void setBackgroundColor(Color color);

	// Gets the window's background color
	Color getBackgroundColor() const;

	// Return window's dimension maximums (in characters).
	int getHorizontal() const;
	int getVertical() const;

	// Return window's dimensions maximums (in pixels).
	int getHorizontalPixels() const;
	int getVerticalPixels() const;

	// Render current window buffer.
	// Return 0 if ok, else -1.
	int swapBuffers();

	// Return pointer to SFML drawing window.
	sf::RenderWindow *getWindow(void) const;
};

}
#endif
