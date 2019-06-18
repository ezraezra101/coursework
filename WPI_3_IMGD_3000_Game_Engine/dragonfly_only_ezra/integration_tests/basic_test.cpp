#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"

#include "Object.h"
#include "EventStep.h"


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
	printf("Running basic test..."); fflush(stdout);
	df::GameManager &game_manager = df::GameManager::getInstance();
	game_manager.startUp();

	
	ObjectTest *ot = new ObjectTest(42);
	ObjectTest *ot2= new ObjectTest(84);

	game_manager.run();

	game_manager.shutDown();

	if(ot->getStepCount() != 42 || ot2->getStepCount() != 42) {
		printf("Didn't run correct number of loops! %d\n", ot->getStepCount());
		return 1;
	} else {
		printf("Done.\n");
	}

	return 0;
}