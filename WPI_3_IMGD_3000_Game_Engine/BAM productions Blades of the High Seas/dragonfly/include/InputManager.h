#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

// Library includes
#include <SFML/Graphics.hpp>

// Engine includes
#include "Manager.h"
#include "Color.h"
#include "Position.h"

namespace df {

class InputManager : public Manager {
	// It's a singleton class, so don't make these public.
	InputManager();
	InputManager(InputManager const &);
	void operator=(InputManager const &);

public:
	~InputManager();

	// Get the ONE instance of InputManager
	static InputManager &getInstance();

	// Get window ready to accept input
	// Return 0 (ok) or -1 (fail)
	int startUp();

	// Revert back to normal window mode.
	void shutDown();

	// Get input from the keyboard and mouse.
	// Pass event along to all Objects.
	void getInput();
};

}
#endif
