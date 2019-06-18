#include "GraphicsManager.h"
#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"

#include "Object.h"
#include "InputManager.h"
#include "EventKeyboard.h"


class Avatar : public df::Object {
public:
	Avatar() {
		setType("Avatar");
		df::GraphicsManager &gm = df::GraphicsManager::getInstance();

		df::Position pos(5,5);
		setPosition(pos);
	}

	int eventHandler(const df::Event *p_e) {
		if(p_e->getType() == df::KEYBOARD_EVENT) {
			const df::EventKeyboard *p_ke = static_cast<const df::EventKeyboard *>(p_e);
			if(p_ke->getAction() == df::KEY_PRESSED) {
				df::Position new_pos = getPosition();
				if(p_ke->getKey() == df::Input::LEFTARROW) {
					new_pos = df::Position(getPosition().getX()-1, getPosition().getY());
				} else if(p_ke->getKey() == df::Input::RIGHTARROW) {
					new_pos = df::Position(getPosition().getX()+1, getPosition().getY());
				} else if(p_ke->getKey() == df::Input::UPARROW) {
					new_pos = df::Position(getPosition().getX(), getPosition().getY()-1);
				} else if(p_ke->getKey() == df::Input::DOWNARROW) {
					new_pos = df::Position(getPosition().getX(), getPosition().getY()+1);
				} else {
					df::GameManager::getInstance().setGameOver();
				}
				df::WorldManager::getInstance().moveObject(this, new_pos);
			}
		}

		return df::Object::eventHandler(p_e);
	}

	void draw(void) {
		df::GraphicsManager & gm = df::GraphicsManager::getInstance();
		gm.drawString(df::Position(40,12), "Press arrow keys to move, any other to quit.", df::CENTER_JUSTIFIED, df::BLUE);
		gm.drawCh(getPosition(), '*', df::WHITE);
	}
};

int main() {
	printf("Running input test..."); fflush(stdout);
	df::GameManager &game_manager = df::GameManager::getInstance();
	game_manager.startUp();

	df::Object *p_o = new Avatar();


	df::WorldManager &wm = df::WorldManager::getInstance();
	wm.setViewPosition(df::Position(5,5));

	df::WorldManager::getInstance().setBoundary(df::Box(
		df::Position(0,0),
		200, 200)
	);
	if(df::WorldManager::getInstance().setViewFollowing(p_o)) {
		printf("Unable to set view to follow object");
	}

	game_manager.run();

	game_manager.shutDown();

	printf("Done.\n");

	return 0;
}