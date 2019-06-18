
#include "Level1.h"
#include "Ground.h"
#include "Instruction.h"
#include "Dolphin.h"

#include "EventKeyboard.h"
#include "LogManager.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

Instruction::Instruction(){
	df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("instruction1");
	if(!sprite) {
		LOG("Instruction sprite not found!");
		return;
	} else {
		setSprite(sprite);
	}
	df::WorldManager &wm = df::WorldManager::getInstance();
	df::Position p = df::Position((wm.getBoundary().getHorizontal() / 2) - 9, (wm.getBoundary().getVertical() / 2) - 3);
	setPosition(p);
	hasSwapped = false;
	hasCreated = false;

	for(int i=0; i<RECOMMENDED_DOLPHIN_COUNT; i++) {
		new Dolphin;
	}
}

int Instruction::eventHandler(const df::Event *p_e) {
	if(p_e->getType() == df::KEYBOARD_EVENT) {
		const df::EventKeyboard *ke = static_cast<const df::EventKeyboard *>(p_e);
		// If we press ESC, quit game
		if(ke->getKey() == df::Input::ESCAPE) {
			df::GameManager::getInstance().setGameOver();
		} else if (ke->getKey() == df::Input::Q && getSprite()->getLabel() == "instruction1" && !hasSwapped){
			df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("instruction2");
			if(!sprite) {
				LOG("Instruction sprite not found!");
				return 0;
			} else {
				setSprite(sprite);
			}
			hasSwapped = true;
		} else if (ke->getKey() == df::Input::U && getSprite()->getLabel() == "instruction2" && !hasCreated){
			new Ground;
			new Level1;
			df::WorldManager::getInstance().markForDelete(this);
			hasCreated = true;
		}
		return 1;
	}

	return 0;
}