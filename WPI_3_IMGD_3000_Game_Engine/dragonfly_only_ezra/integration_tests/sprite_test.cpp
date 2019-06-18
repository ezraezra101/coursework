#include "GameManager.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

#include "Object.h"
#include "EventStep.h"

#define SPRITE_FOLDER "unit_tests/sprites/"

#define MAX_SPRITE_SLOWDOWN 4

// Quits the game after a certain number of game loops.
class ObjectTest : public df::Object {
private:
	int max_step_count;
	int slowdown;
public:
	ObjectTest(int init_max_step_count) {
		setType("ObjectTest");
		max_step_count = init_max_step_count;
		slowdown = -10;
	}

	// End the game if the game loop has run enough.
	int eventHandler(const df::Event *p_e) {
		if(p_e->getType() == df::STEP_EVENT) {
			const df::EventStep *p_s = static_cast<const df::EventStep *> (p_e);

			df::LogManager::getInstance().writeLog("ObjectTest %d recieved step event (step %d)!", getId(), p_s->getStepCount());

			slowdown++;
			setSpriteSlowdown( (MAX_SPRITE_SLOWDOWN + (slowdown % MAX_SPRITE_SLOWDOWN)) % MAX_SPRITE_SLOWDOWN);

			if(p_s->getStepCount() >= max_step_count-1) { // -1 because steps start at 0.
				df::GameManager::getInstance().setGameOver();
			}
			return 1;
		}
		return 0;
	}
};


int main() {
	printf("Running sprite test..."); fflush(stdout);
	df::GameManager &game_manager = df::GameManager::getInstance();
	game_manager.startUp();

	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	resource_manager.loadSprite(SPRITE_FOLDER "bullet-spr.txt", "bullet");
	resource_manager.loadSprite(SPRITE_FOLDER "explosion-spr.txt", "explosion");
	resource_manager.loadSprite(SPRITE_FOLDER "gameover-spr.txt", "gameover");
	resource_manager.loadSprite(SPRITE_FOLDER "gamestart-spr.txt", "gamestart");
	resource_manager.loadSprite(SPRITE_FOLDER "saucer-spr.txt", "saucer");
	resource_manager.loadSprite(SPRITE_FOLDER "ship-spr.txt", "ship");
	
	ObjectTest *ot = new ObjectTest(42);
	ot->setPosition(df::Position(2,2));

	ot->setSprite(resource_manager.getSprite("gamestart"));

	//ot->setSpriteSlowdown(10);

	game_manager.run();

	game_manager.shutDown();

	printf("Done. Should have shown GAME START flickering.\n");

	return 0;
}