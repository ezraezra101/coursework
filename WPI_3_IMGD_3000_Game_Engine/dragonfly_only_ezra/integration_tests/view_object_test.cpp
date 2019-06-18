#include "GameManager.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "Color.h"
#include "Object.h"
#include "EventKeyboard.h"
#include "EventOut.h"
#include "EventStep.h"
#include "EventCollision.h"
#include "ViewObject.h"

// Quits the game after a certain number of game loops.
class ObjectTest : public df::Object {
private:
	int max_step_count;
	int step_count;
	int count2; // Why on earth does defining this fix things?!
public:
	ObjectTest(int init_max_step_count) {
		setType("ObjectTest");
		max_step_count = init_max_step_count;
		step_count = 0;
	}

	// End the game if the game loop has run enough.
	int eventHandler(const df::Event *p_e) {
		if(p_e->getType() == df::STEP_EVENT) {
			const df::EventStep *p_s = static_cast<const df::EventStep *> (p_e);
			step_count++;

			df::LogManager::getInstance().writeLog("ObjectTest %d recieved step event (step %d)!", getId(), p_s->getStepCount());

			if(p_s->getStepCount() >= max_step_count-1) { // -1 because steps start at 0.
				df::GameManager::getInstance().setGameOver();
			}
			return 1;
		}
		return 0;
	}

	int getStepCount() {
		return step_count;
	}
};

int main() {
	printf("Running view object test..."); fflush(stdout);
	df::GameManager &game_manager = df::GameManager::getInstance();
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	game_manager.startUp();

	new ObjectTest(42);

	df::ViewObject *v_o = new df::ViewObject();
	v_o->setViewString("Hello");

	printf("%d, %d\n", v_o->getPosition().getX(), v_o->getPosition().getY());

	game_manager.run();

	game_manager.shutDown();

	printf("Done.\n");

	return 0;
}