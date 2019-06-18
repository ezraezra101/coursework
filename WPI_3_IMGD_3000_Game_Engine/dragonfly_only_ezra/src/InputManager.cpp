#include "InputManager.h"
#include "GraphicsManager.h"
#include "LogManager.h"

#include "EventKeyboard.h"
#include "EventMouse.h"


// Convert keys from SFML format to Dragonfly format.
static inline df::Input::Key sfKeyToDfKey(sf::Keyboard::Key key) {
	switch(key) {
	case sf::Keyboard::Unknown:
		return df::Input::UNDEFINED_KEY;
	case sf::Keyboard::Space:
		return df::Input::SPACE;
	case sf::Keyboard::Return:
		return df::Input::RETURN;
	case sf::Keyboard::Escape:
		return df::Input::ESCAPE;
	case sf::Keyboard::Tab:
		return df::Input::TAB;
	case sf::Keyboard::Left:
		return df::Input::LEFTARROW;
	case sf::Keyboard::Right:
		return df::Input::RIGHTARROW;
	case sf::Keyboard::Up:
		return df::Input::UPARROW;
	case sf::Keyboard::Down:
		return df::Input::DOWNARROW;
	case sf::Keyboard::Pause:
		return df::Input::PAUSE;
	case sf::Keyboard::Dash:
		return df::Input::MINUS;
	case sf::Keyboard::Equal:
		return df::Input::PLUS;
	case sf::Keyboard::Tilde:
		return df::Input::TILDE;
	case sf::Keyboard::Period:
		return df::Input::PERIOD;
	case sf::Keyboard::Comma:
		return df::Input::COMMA;
	case sf::Keyboard::Slash:
		return df::Input::SLASH;
	case sf::Keyboard::LControl:
		return df::Input::LEFTCONTROL;
	case sf::Keyboard::RControl:
		return df::Input::RIGHTCONTROL;
	case sf::Keyboard::LShift:
		return df::Input::LEFTSHIFT;
	case sf::Keyboard::RShift:
		return df::Input::RIGHTSHIFT;
	case sf::Keyboard::F1:
		return df::Input::F1;
	case sf::Keyboard::F2:
		return df::Input::F2;
	case sf::Keyboard::F3:
		return df::Input::F3;
	case sf::Keyboard::F4:
		return df::Input::F4;
	case sf::Keyboard::F5:
		return df::Input::F5;
	case sf::Keyboard::F6:
		return df::Input::F6;
	case sf::Keyboard::F7:
		return df::Input::F7;
	case sf::Keyboard::F8:
		return df::Input::F8;
	case sf::Keyboard::F9:
		return df::Input::F9;
	case sf::Keyboard::F10:
		return df::Input::F10;
	case sf::Keyboard::F11:
		return df::Input::F11;
	case sf::Keyboard::F12:
		return df::Input::F12;
	case sf::Keyboard::A:
		return df::Input::A;
	case sf::Keyboard::B:
		return df::Input::B;
	case sf::Keyboard::C:
		return df::Input::C;
	case sf::Keyboard::D:
		return df::Input::D;
	case sf::Keyboard::E:
		return df::Input::E;
	case sf::Keyboard::F:
		return df::Input::F;
	case sf::Keyboard::G:
		return df::Input::G;
	case sf::Keyboard::H:
		return df::Input::H;
	case sf::Keyboard::I:
		return df::Input::I;
	case sf::Keyboard::J:
		return df::Input::J;
	case sf::Keyboard::K:
		return df::Input::K;
	case sf::Keyboard::L:
		return df::Input::L;
	case sf::Keyboard::M:
		return df::Input::M;
	case sf::Keyboard::N:
		return df::Input::N;
	case sf::Keyboard::O:
		return df::Input::O;
	case sf::Keyboard::P:
		return df::Input::P;
	case sf::Keyboard::Q:
		return df::Input::Q;
	case sf::Keyboard::R:
		return df::Input::R;
	case sf::Keyboard::S:
		return df::Input::S;
	case sf::Keyboard::T:
		return df::Input::T;
	case sf::Keyboard::U:
		return df::Input::U;
	case sf::Keyboard::V:
		return df::Input::V;
	case sf::Keyboard::W:
		return df::Input::W;
	case sf::Keyboard::X:
		return df::Input::X;
	case sf::Keyboard::Y:
		return df::Input::Y;
	case sf::Keyboard::Z:
		return df::Input::Z;
	case sf::Keyboard::Num1:
		return df::Input::NUM1;
	case sf::Keyboard::Num2:
		return df::Input::NUM2;
	case sf::Keyboard::Num3:
		return df::Input::NUM3;
	case sf::Keyboard::Num4:
		return df::Input::NUM4;
	case sf::Keyboard::Num5:
		return df::Input::NUM5;
	case sf::Keyboard::Num6:
		return df::Input::NUM6;
	case sf::Keyboard::Num7:
		return df::Input::NUM7;
	case sf::Keyboard::Num8:
		return df::Input::NUM8;
	case sf::Keyboard::Num9:
		return df::Input::NUM9;
	case sf::Keyboard::Num0:
		return df::Input::NUM0;
	default:
		return df::Input::UNDEFINED_KEY;
	}
}

// Converts mouse button from SFML format to Dragonfly format.
static inline df::EventMouseButton sfMouseButtonToDfMouseButton(sf::Mouse::Button button) {
	switch(button) {
	case sf::Mouse::Right:
		return df::RIGHT;
	case sf::Mouse::Left:
		return df::LEFT;
	case sf::Mouse::Middle:
		return df::MIDDLE;
	default:
		return df::UNDEFINED_MOUSE_BUTTON;
	}
}


// Constructor
df::InputManager::InputManager() {
}

//Just overriding the default functions:
df::InputManager::InputManager(const df::InputManager &other) {}
void df::InputManager::operator=(const df::InputManager &other) {}



df::InputManager::~InputManager() {

}

df::InputManager &df::InputManager::getInstance() {
	static df::InputManager input_manager;
	return input_manager;
}

int df::InputManager::startUp() {
	df::GraphicsManager &gm = df::GraphicsManager::getInstance();
	if(not gm.isStarted()) {
		LOG("InputManager can't start without GraphicsManager already started!");
		return -1;
	}
	sf::RenderWindow* p_window = gm.getWindow();
	p_window->setKeyRepeatEnabled(false); // Disable keybard repeat.

	return Manager::startUp();
}

void df::InputManager::shutDown() {

}

// Get input from the keyboard and mouse.
// Pass event along to all Objects.
void df::InputManager::getInput() {
	if(!isStarted()) {
		LOG("InputManager can't get input without being started!");
		return;
	}

	sf::Event event;
	sf::RenderWindow *p_window = df::GraphicsManager::getInstance().getWindow();

	while(p_window->pollEvent(event)) {
		df::EventKeyboard ke;
		df::EventMouse me;
		df::Position mouse_xy;
		switch(event.type) {
		case sf::Event::KeyPressed: // Key was pressed
			ke = df::EventKeyboard(sfKeyToDfKey(event.key.code), df::KEY_PRESSED);
			onEvent(&ke);
			break;
		case sf::Event::KeyReleased: // Key released
			ke = df::EventKeyboard(sfKeyToDfKey(event.key.code), df::KEY_RELEASED);
			onEvent(&ke);
			break;
		case sf::Event::MouseMoved: // Mouse was moved.
			mouse_xy.setXY(event.mouseMove.x, event.mouseMove.y);
			me = df::EventMouse(df::UNDEFINED_MOUSE_BUTTON, df::MOVED, mouse_xy);
			onEvent(&me);
			break;
		case sf::Event::MouseButtonPressed: // Mouse was clicked
			mouse_xy.setXY(event.mouseButton.x, event.mouseButton.y);
			me = df::EventMouse(sfMouseButtonToDfMouseButton(event.mouseButton.button), df::CLICKED, mouse_xy);
			onEvent(&me);
			break;
		default:
			break;
		}
	}
	// Key was pressed
	const int key_count = 67; // 68 different keys, including 'Unknown'
	sf::Keyboard::Key key_list[] = {
		/* sf::Keyboard::Unknown, -- causes segfaults */ sf::Keyboard::Space, sf::Keyboard::Return, sf::Keyboard::Escape, sf::Keyboard::Tab, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Pause, sf::Keyboard::Dash, sf::Keyboard::Equal, sf::Keyboard::Tilde, sf::Keyboard::Period, sf::Keyboard::Comma, sf::Keyboard::Slash, sf::Keyboard::LControl, sf::Keyboard::RControl, sf::Keyboard::LShift, sf::Keyboard::RShift, sf::Keyboard::F1, sf::Keyboard::F2, sf::Keyboard::F3, sf::Keyboard::F4, sf::Keyboard::F5, sf::Keyboard::F6, sf::Keyboard::F7, sf::Keyboard::F8, sf::Keyboard::F9, sf::Keyboard::F10, sf::Keyboard::F11, sf::Keyboard::F12, sf::Keyboard::A, sf::Keyboard::B, sf::Keyboard::C, sf::Keyboard::D, sf::Keyboard::E, sf::Keyboard::F, sf::Keyboard::G, sf::Keyboard::H, sf::Keyboard::I, sf::Keyboard::J, sf::Keyboard::K, sf::Keyboard::L, sf::Keyboard::M, sf::Keyboard::N, sf::Keyboard::O, sf::Keyboard::P, sf::Keyboard::Q, sf::Keyboard::R, sf::Keyboard::S, sf::Keyboard::T, sf::Keyboard::U, sf::Keyboard::V, sf::Keyboard::W, sf::Keyboard::X, sf::Keyboard::Y, sf::Keyboard::Z, sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4, sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8, sf::Keyboard::Num9, sf::Keyboard::Num0
	};
	for(int i=0; i<key_count; i++) {
		if(sf::Keyboard::isKeyPressed(key_list[i])) {
			df::EventKeyboard kpe(sfKeyToDfKey(key_list[i]), df::KEY_DOWN);
			onEvent(&kpe);
		}
	}
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		df::Position blank_mouse_xy;
		df::EventMouse mpe(df::LEFT, df::PRESSED, blank_mouse_xy);
		onEvent(&mpe);
	}
	if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		df::Position blank_mouse_xy_2;
		df::EventMouse mpe(df::MIDDLE, df::PRESSED, blank_mouse_xy_2);
		onEvent(&mpe);
	}
	if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		df::Position blank_mouse_xy_3;
		df::EventMouse mpe(df::RIGHT, df::PRESSED, blank_mouse_xy_3);
		onEvent(&mpe);
	}

}