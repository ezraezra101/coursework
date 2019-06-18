#include "GraphicsManager.h"
#include "WorldManager.h"
#include "ObjectListIterator.h"

int main() {
	int failed_tests = 0;

	df::Object *o, *o2;

	df::WorldManager &world_manager = df::WorldManager::getInstance();

	if(world_manager.startUp()) {
		failed_tests++;
		printf("WorldManager::startUp() failed.\n");
	}
	if(!world_manager.getAllObjects().isEmpty()) {
		failed_tests++;
		printf("WorldManager doesn't initialize as empty.\n");
	}

	o = new df::Object();
	o2 = new df::Object();
	if(world_manager.getAllObjects().getCount() != 2) {
		failed_tests++;
		printf("WorldManager can't insert objects!\n");
	}
	world_manager.shutDown();
	if(!world_manager.getAllObjects().isEmpty()) {
		failed_tests++;
		printf("WorldManager::shutDown() didn't empty world.\n");
	}

	world_manager.startUp();

	if(!world_manager.isStarted()) {
		failed_tests++;
		printf("WorldManager doesn't think it's started!\n");
	}

	o = new df::Object();
	o2 = new df::Object();

	if(world_manager.getAllObjects().getCount() != 2
	   || world_manager.removeObject(o)
	   || world_manager.getAllObjects().getCount() != 1) {
	   
		failed_tests++;
		printf("WorldManager didn't remove objects properly.\n");
	}

	world_manager.shutDown();
	world_manager.startUp();

	o2 = new df::Object();

	if(world_manager.markForDelete(o2)
		|| world_manager.getAllObjects().getCount() != 1) {
	
		failed_tests++;
		printf("WorldManager deleted an object when marked for delete.\n");
	}
	world_manager.update(); // Should delete o2
	

	if(!world_manager.getAllObjects().isEmpty()) {
		failed_tests++;
		printf("WorldManager::markForDelete() didn't delete object on update.\n");
	}

	if( world_manager.getBoundary().getHorizontal() != df::WINDOW_HORIZONTAL_CHARS_DEFAULT ||
		world_manager.getBoundary().getVertical() != df::WINDOW_VERTICAL_CHARS_DEFAULT ||
		world_manager.getBoundary().getCorner().getX() != 0
		) {
		failed_tests++;
		printf("WorldManager doesn't have the right default boundary! Actual: (%d, %d)\n",
			world_manager.getBoundary().getHorizontal(),
			world_manager.getBoundary().getVertical());
	}

	world_manager.setBoundary(df::Box());
	world_manager.setView(df::Box());
	if((world_manager.getBoundary().getHorizontal() != 0 ||
		world_manager.getView().getHorizontal() != 0)) {
		failed_tests++;
		printf("WorldManager boundary & view setters don't work!\n");
	}

	// Testing update(), isCollision(), moveObject(), and draw() in integration tests.

	// SetViewFollowing is tested elsewhere.

	world_manager.shutDown();

	return failed_tests;
}