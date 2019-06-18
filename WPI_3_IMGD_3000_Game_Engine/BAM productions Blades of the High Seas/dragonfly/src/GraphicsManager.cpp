// Library includes
#include <SFML/Graphics.hpp>

// Engine includes
#include "GraphicsManager.h"
#include "LogManager.h"
#include "utility.h"


///////////////////
// Handy Helpers //
///////////////////

static inline sf::Color dragonflyColorToSFMLColor(df::Color color) {
	switch(color) {
	case df::BLACK:
		return sf::Color::Black;
	case df::RED:
		return sf::Color::Red;
	case df::GREEN:
		return sf::Color::Green;
	case df::YELLOW:
		return sf::Color::Yellow;
	case df::BLUE:
		return sf::Color::Blue;
	case df::MAGENTA:
		return sf::Color::Magenta;
	case df::CYAN:
		return sf::Color::Cyan;
	case df::WHITE:
		return sf::Color::White;
	default:
		return dragonflyColorToSFMLColor(df::COLOR_DEFAULT);
	}
}


// Compute character height, based on window size and font.
float df::GraphicsManager::charHeight() const {
	return (float) window_vertical_pixels / window_vertical_chars;
}

// Compute character width, based on window size and font.
float df::GraphicsManager::charWidth() const {
	return (float) window_horizontal_pixels / window_horizontal_chars;
}

// Convert ASCII space (x,y) to window pixels (x,y).
df::Position df::GraphicsManager::spacesToPixels(df::Position spaces) const {
	return df::Position(spaces.getX() * charWidth(), spaces.getY() * charHeight());
}

// Convert window pixels (x,y) to ASCII spaces (x,y).
df::Position df::GraphicsManager::pixelsToSpaces(df::Position pixels) const {
	return df::Position(pixels.getX()/charWidth(), pixels.getY() /charHeight());
}

df::GraphicsManager::GraphicsManager() {
	setType("GraphicsManager");
	font = sf::Font();
	p_window = NULL;
	window_horizontal_pixels = df::WINDOW_HORIZONTAL_PIXELS_DEFAULT;
	window_vertical_pixels = df::WINDOW_VERTICAL_PIXELS_DEFAULT;
	window_horizontal_chars = df::WINDOW_HORIZONTAL_CHARS_DEFAULT;
	window_vertical_chars = df::WINDOW_VERTICAL_CHARS_DEFAULT;
	window_background_color = df::WINDOW_BACKGROUND_COLOR_DEFAULT;
}

// Private constructory things. Prevents people from copying GraphicsManager
df::GraphicsManager::GraphicsManager(const df::GraphicsManager &other) {
	setType("GraphicsManager");
}
void df::GraphicsManager::operator=(const df::GraphicsManager &other) {
	setType("GraphicsManager");
}

df::GraphicsManager::~GraphicsManager() {
	shutDown();
}


// Get the ONE instance of GraphicsManager
df::GraphicsManager &df::GraphicsManager::getInstance() {
	static df::GraphicsManager graphics_manager = GraphicsManager();
	return graphics_manager;
}

// Start up graphics window ready for text-based display
// Return 0 (ok) or -1 (fail) 
int df::GraphicsManager::startUp() {

	if(p_window != NULL) {
		// Window already created... do nothing
		return 0;
	}

	// Create window
	p_window = new sf::RenderWindow(sf::VideoMode(window_horizontal_pixels, window_vertical_pixels), WINDOW_TITLE_DEFAULT, WINDOW_STYLE_DEFAULT);
	
	p_window->setMouseCursorVisible(false); // Turn off mouse cursor

	p_window->setVerticalSyncEnabled(true);// Synchronize refresh rate with monitor

	int loaded_font = font.loadFromFile(FONT_FILE_DEFAULT); // Load font

	if( p_window != NULL && loaded_font && !isStarted()) {
		LOG("GraphicsManager started successfully");
		return Manager::startUp();
	} else {
		if(p_window == NULL) {
			LOG("GraphicsManager failed to start: SFML window didn't initialize.");
		} else {
			LOG("Font failed to load!");
		}
		p_window = NULL;
		return -1;
	}
}

// Close graphics window
void df::GraphicsManager::shutDown() {
	if(p_window != NULL) {
		p_window->close();
		delete p_window;
		p_window = NULL;
	}

	Manager::shutDown();
}
// Draw character at screen location (x,y) with color.
// Return 0 if ok, -1 if not.
int df::GraphicsManager::drawCh(df::Position world_pos, char ch, df::Color color) const {
	if(p_window == NULL) {
		LOG("Called GraphicsManager::drawCh without a window!");
		return -1;
	}

	// Convert space (x,y) to pixels.
	Position pixel_pos = spacesToPixels(worldToView(world_pos));

	// Draw background rectangle since text is "see through" in SFML.
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(charWidth(), charHeight()-1)); // -1 is magic
	rectangle.setFillColor(dragonflyColorToSFMLColor(window_background_color));
	rectangle.setPosition(pixel_pos.getX() + charWidth()/2 - df::MAGIC_RECTANGLE_OFFSET_X,
		pixel_pos.getY() + charHeight()/2 - df::MAGIC_RECTANGLE_OFFSET_Y);

	p_window->draw(rectangle);

	// Create character text to draw.
	static sf::Text text("", font);
	text.setString(ch);
	text.setStyle(sf::Text::Bold); // Bold text happens to look better.

	// Scale to right size.
	if(charWidth() < charHeight()) {
		text.setCharacterSize(charWidth()*2);
	} else {
		text.setCharacterSize(charHeight()*2);
	}

	text.setColor(dragonflyColorToSFMLColor(color));

	text.setPosition(pixel_pos.getX(), pixel_pos.getY());

	p_window->draw(text);

	return 0;
}

// Draw string at window location (x,y)
// Justified left, center, or right.
// Return 0 if OK, else -1.
int df::GraphicsManager::drawString(Position pos, std::string str, Justification just, Color color) const {
	// Get starting position.
	Position starting_pos = pos;
	switch(just) {
	case CENTER_JUSTIFIED:
		starting_pos.setX(pos.getX() - str.size()/2);
		break;
	case RIGHT_JUSTIFIED:
		starting_pos.setX(pos.getX() - str.size());
	case LEFT_JUSTIFIED:
	default:
		break;
	}

	// Draw string character by character.
	for(int i=0; i<str.size(); i++) {
		Position temp_pos(starting_pos.getX() + i, starting_pos.getY());
		if(drawCh(temp_pos, str[i], color)) {
			return -1; // If we have trouble drawing a character.
		}
	}

	return 0;
}

// Draw single sprite frame aty window location (x, y) with color.
// If centered true then center frame at (x, y).
// Return 0 if ok, else -1
int df::GraphicsManager::drawFrame(Position world_pos,
	Frame frame, bool centered, Color color) const {
	if(frame.getString() == "") {
		return -1;
	}

	if(centered) {
		world_pos = Position(world_pos.getX() + frame.getWidth()/2,
			world_pos.getY() + frame.getHeight()/2);
	}

	// Frame data stored in string
	std::string str = frame.getString();
	for(int y=0; y<frame.getHeight(); y++) {
		for(int x=0; x<frame.getWidth(); x++) {
			df::Position temp_pos(world_pos.getX()+x, world_pos.getY()+y);
			if(drawCh(temp_pos, str[y * frame.getWidth()+x], color)) {
				return -1;
			}
		}
	}

	return 0;
}

// Sets the window's background color
void df::GraphicsManager::setBackgroundColor(df::Color color) {
	window_background_color = color;
}

// Gets the window's background color
df::Color df::GraphicsManager::getBackgroundColor() const {
	return window_background_color;
}


// Return window's dimension maximums (in characters).
int df::GraphicsManager::getHorizontal() const {
	return window_horizontal_chars;
}
int df::GraphicsManager::getVertical() const {
	return window_vertical_chars;
}

// Return window's dimensions maximums (in pixels).
int df::GraphicsManager::getHorizontalPixels() const {
	return window_horizontal_pixels;
}
int df::GraphicsManager::getVerticalPixels() const {
	return window_vertical_pixels;
}

// Render current window buffer.
// Return 0 if ok, else -1.
int df::GraphicsManager::swapBuffers() {
	if(p_window) {
		p_window->display(); // Swaps buffers & displays current window.
		p_window->clear(dragonflyColorToSFMLColor(window_background_color)); // Get ready for next draw.
		return 0;
	} else {
		LOG("Called GraphicsManager::swapBuffers without a window!");
		return -1;
	}

}

// Return pointer to SFML drawing window.
sf::RenderWindow *df::GraphicsManager::getWindow(void) const {
	return p_window;
}