#include "GraphicsManager.h"
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

	void draw(void) {
		df::GraphicsManager & gm = df::GraphicsManager::getInstance();
		gm.drawString(df::Position(0,0), "Hello world!", df::LEFT_JUSTIFIED, df::BLUE);
		df::Position bottom_right_corner(df::WINDOW_HORIZONTAL_CHARS_DEFAULT-1, df::WINDOW_VERTICAL_CHARS_DEFAULT-1);
		gm.drawCh(bottom_right_corner,'X', df::RED);

		gm.drawCh(df::Position(step_count, df::WINDOW_VERTICAL_CHARS_DEFAULT-1), 'U', df::CYAN);
	}
};


int main() {
	printf("Running graphics test..."); fflush(stdout);
	df::GameManager &game_manager = df::GameManager::getInstance();
	game_manager.startUp();

	df::GraphicsManager::getInstance().setBackgroundColor(df::WHITE);
	
	ObjectTest *ot = new ObjectTest(64);

	game_manager.run();

	game_manager.shutDown();


	printf("Done. Should have shown 'Hello world!', an 'X', and a moving 'U'\n");

	return 0;
}