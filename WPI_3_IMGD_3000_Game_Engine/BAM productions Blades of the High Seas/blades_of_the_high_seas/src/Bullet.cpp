#include "EventCollision.h"
#include "EventOut.h"
#include "ResourceManager.h"
#include "WorldManager.h"


#include "Bullet.h"
#include "EventLevelUp.h"


Bullet::Bullet(FACING direction, df::Position p, df::Color new_color) {
	df::ResourceManager &rm = df::ResourceManager::getInstance();
	df::WorldManager &wm = df::WorldManager::getInstance();

	dir = direction;
	setPosition(p);
	setType("Bullet");

	color = new_color;

	if(dir == left) {
		setXVelocity(-0.5);

		setSprite(rm.getSprite("bullet_left"));

	} else {
		setXVelocity(0.5);

		setSprite(rm.getSprite("bullet_right"));
	}
	setSpriteSlowdown(4);

}


int Bullet::eventHandler(const df::Event *p_e) {
	if(p_e->getType() == LEVEL_UP_EVENT) {
		df::WorldManager &wm = df::WorldManager::getInstance();
		wm.markForDelete(this);
		return 1;
	} else if( p_e->getType() == df::COLLISION_EVENT ||
		p_e->getType() == df::OUT_EVENT) {
		df::WorldManager &wm = df::WorldManager::getInstance();
		wm.markForDelete(this);
		return 1;
	}
	return df::Object::eventHandler(p_e);
}

bool Bullet::headingTowards(df::Object *p_o) {
	int start_x = p_o->getPosition().getX() + p_o->getBox().getCorner().getX();
	int end_x = start_x + p_o->getBox().getHorizontal();
	return	(dir == left && start_x < getPosition().getX()) ||
			(dir == right && end_x > getPosition().getX());
}

void Bullet::draw() {
	getSprite()->setColor(color);
	df::Object::draw();
}