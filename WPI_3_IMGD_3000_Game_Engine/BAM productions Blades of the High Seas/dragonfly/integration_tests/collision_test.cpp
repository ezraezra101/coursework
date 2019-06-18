#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "utility.h"

#include "Object.h"
#include "EventCollision.h"
#include "EventOut.h"

int global_collision_count = 0;
int global_out_count = 0;

class Collidable : public df::Object {

public:
	Collidable() {
		setType("Collidable");
	}
	int eventHandler(const df::Event *p_e) {
		if(p_e->getType() == df::COLLISION_EVENT) {
			const df::EventCollision *p_ce = static_cast<const df::EventCollision *>(p_e);
			LOG("Collision occured at (%d, %d) between %d and %d", p_ce->getPosition().getX(), p_ce->getPosition().getY(), p_ce->getObject1()->getId(), p_ce->getObject2()->getId());
			global_collision_count++;
			return 1;
		} else if(p_e->getType() == df::OUT_EVENT) {
			LOG("Object %d left the stage.\n", getId());
			global_out_count++;
			return 1;
		} else {
			return df::Object::eventHandler(p_e);
		}
	}
};

int main() {
	printf("Running collision test..."); fflush(stdout);
	df::GameManager &game_manager = df::GameManager::getInstance();
	game_manager.startUp();

	df::Object *o1 = new Collidable();
	df::Object *o2 = new Collidable();

	o2->setPosition(df::Position(3,3));

	df::WorldManager &world_manager = df::WorldManager::getInstance();

	if(world_manager.isCollision(o1, o2->getPosition()).isEmpty()) {
		printf("WorldManager::isCollision never collides.\n");
		return 1;
	}

	if(!world_manager.isCollision(o1, o1->getPosition()).isEmpty()) {
		printf("WorldManager::isCollision allows objects to collide with themselves!.\n");
		return 1;
	}

	if(world_manager.moveObject(o1, o2->getPosition())) {
		printf("WorldManager::moveObject didn't allow two soft objects to overlap.\n");
		return 1;
	}
	if(o1->getPosition().getX() != o2->getPosition().getX()) {
		printf("Object not moved by WorldManager::moveObject()\n");
		return 1;
	}
	if(global_collision_count != 2) {
		printf("WorldManager::moveObject() didn't send out proper numbers of collisions! (actual: %d expected: 2)\n", global_collision_count);
		return 1;
	}

	// Testing outEvent:
	if(world_manager.moveObject(o2, df::Position(-1,0)) ||
		global_out_count != 1) {
		printf("WorldManager::moveObject() doesn't send out events!\n");
		return 1;
	}


	game_manager.shutDown();

	printf("Done.\n");

	return 0;
}