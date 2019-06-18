#include "GraphicsManager.h"
#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"

#include "Object.h"
#include "EventStep.h"
#include "Clock.h"


class Star : public df::Object {
public:
	Star() {
		setType("Star");
		df::GraphicsManager &gm = df::GraphicsManager::getInstance();

		df::Position pos(rand() % gm.getHorizontal(), rand() % gm.getVertical());
		setPosition(pos);

		setAltitude(0);
	}

	void draw(void) {
		df::GraphicsManager & gm = df::GraphicsManager::getInstance();
		gm.drawCh(getPosition(), '*', df::WHITE);
	}
};



class TimerObject : public df::Object {
private:
	int max_step_count;
	int step_count;
	df::Clock clock;
public:
	TimerObject(int init_max_step_count) {
		df::Object::setType("TimerObject");
		max_step_count = init_max_step_count;
		step_count = 0;
		clock.delta();
	}

	// End the game if the game loop has run enough.
	int eventHandler(const df::Event *p_e) {
		if(p_e->getType() == df::STEP_EVENT) {
			const df::EventStep *p_s = static_cast<const df::EventStep *> (p_e);

			step_count = p_s->getStepCount();

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

		std::string s = std::to_string(step_count);
		gm.drawString(df::Object::getPosition(), s, df::CENTER_JUSTIFIED, df::RED);

		int loop_time_imperfection = clock.split() - df::FRAME_TIME_DEFAULT*1000 * step_count;
		int acceptable_deviation = df::FRAME_TIME_DEFAULT*1000;
		if(loop_time_imperfection > acceptable_deviation || loop_time_imperfection < -acceptable_deviation) {
			gm.drawString(df::Position(getPosition().getX(),getPosition().getY()+1),
				"Timing is off", df::RIGHT_JUSTIFIED, df::RED);
		} else {
			gm.drawString(df::Position(getPosition().getX(),getPosition().getY()+1),
				"Timing is OK!", df::RIGHT_JUSTIFIED, df::GREEN);
		}
	}
};

int main() {
	printf("Running graphics test 2..."); fflush(stdout);
	df::GameManager &game_manager = df::GameManager::getInstance();
	game_manager.startUp();

	for(int i=0; i< 128; i++) {
		new Star();
	}

	TimerObject *p_t = new TimerObject(128);
	df::Position pos(40, 12);
	p_t->setPosition(pos);

	game_manager.run();

	game_manager.shutDown();


	printf("Done.\n");

	return 0;
}