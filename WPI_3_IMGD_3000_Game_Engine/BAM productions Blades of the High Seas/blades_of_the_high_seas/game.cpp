// Engine includes
#include "GameManager.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "ResourceManager.h"

// Game includes
#include "Instruction.h"
#include "LevelWin.h"

#define SPRITE_FOLDER "sprites/"
#define SOUNDS_FOLDER "sounds/"

void loadResources();
void populateWorld();


int main() {
	df::GameManager &game_manager = df::GameManager::getInstance();
	df::LogManager::getInstance().startUp(); // Startup log manager in case game manager fails to startup.

	df::LogManager::getInstance().setFlush();

	if(game_manager.startUp()) {
		LOG("Game Manager didn't start up!");
		game_manager.shutDown();
		return 1;
	}

	df::GraphicsManager::getInstance().setBackgroundColor(df::BLUE);

	loadResources();
	populateWorld();

	df::ResourceManager::getInstance().getMusic("theme")->play(true);
	game_manager.run();

	game_manager.shutDown();

	return 0;
}


void loadResources() {
	df::ResourceManager &rm = df::ResourceManager::getInstance();
	
	int not_ok = false;

	// Load sprites
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Instruction1.txt", "instruction1");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Instruction2.txt", "instruction2");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "The_Gunwale.txt", "the_gunwale");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "The_Forecastle.txt", "the_forecastle");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "The_Deck.txt", "the_deck");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "The_Brig.txt", "the_brig");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "The_Hold.txt", "the_hold");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Chest.txt", "winrar");

	not_ok |= rm.loadSprite(SPRITE_FOLDER "Bullet_Left.txt", "bullet_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Bullet_Right.txt", "bullet_right");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Dolphin.txt", "dolphin");

	// Hero 1:
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1.txt", "pirate1");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Walk_Left.txt", "pirate1_walk_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Walk_Right.txt", "pirate1_walk_right");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Stab_Left.txt", "pirate1_stab_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Stab_Right.txt", "pirate1_stab_right");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Shoot_Left.txt", "pirate1_shoot_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Shoot_Right.txt", "pirate1_shoot_right");

	// Hero 2:
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1.txt", "pirate2");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Walk_Left.txt", "pirate2_walk_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Walk_Right.txt", "pirate2_walk_right");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Stab_Left.txt", "pirate2_stab_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Stab_Right.txt", "pirate2_stab_right");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Shoot_Left.txt", "pirate2_shoot_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Shoot_Right.txt", "pirate2_shoot_right");
	if(not_ok) {
		printf("Failed to load resource!\nSee log for more details.\n");
	}
	rm.getSprite("pirate2")->setColor(df::YELLOW);
	rm.getSprite("pirate2_walk_left")->setColor(df::YELLOW);
	rm.getSprite("pirate2_walk_right")->setColor(df::YELLOW);
	rm.getSprite("pirate2_stab_left")->setColor(df::YELLOW);
	rm.getSprite("pirate2_stab_right")->setColor(df::YELLOW);
	rm.getSprite("pirate2_shoot_left")->setColor(df::YELLOW);
	rm.getSprite("pirate2_shoot_right")->setColor(df::YELLOW);

	// Enemies:

	// Basic enemy:
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Hat.txt", "foe");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Hat_Walk_Left.txt", "foe_walk_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Hat_Walk_Right.txt", "foe_walk_right");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Hat_Stab_Left.txt", "foe_stab_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Hat_Stab_Right.txt", "foe_stab_right");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Hat_Shoot_Left.txt", "foe_shoot_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Hat_Shoot_Right.txt", "foe_shoot_right");

	// Corporal:
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1.txt", "corporal");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Walk_Left.txt", "corporal_walk_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Walk_Right.txt", "corporal_walk_right");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Stab_Left.txt", "corporal_stab_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Stab_Right.txt", "corporal_stab_right");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Shoot_Left.txt", "corporal_shoot_left");
	not_ok |= rm.loadSprite(SPRITE_FOLDER "Pirate1_Shoot_Right.txt", "corporal_shoot_right");
	rm.getSprite("corporal")->setColor(df::RED);
	rm.getSprite("corporal_walk_left")->setColor(df::RED);
	rm.getSprite("corporal_walk_right")->setColor(df::RED);
	rm.getSprite("corporal_stab_left")->setColor(df::RED);
	rm.getSprite("corporal_stab_right")->setColor(df::RED);
	rm.getSprite("corporal_shoot_left")->setColor(df::RED);
	rm.getSprite("corporal_shoot_right")->setColor(df::RED);


	not_ok |= rm.loadSprite(SPRITE_FOLDER "Fancy_Ground.txt", "ground");

	// Load sounds
	not_ok |= rm.loadMusic(SOUNDS_FOLDER "Piece_For_Game.ogg", "theme");
	not_ok |= rm.loadSound(SOUNDS_FOLDER "Gunshot.wav", "gunshot");
	not_ok |= rm.loadSound(SOUNDS_FOLDER "Swing.wav", "swing");

	if(not_ok) {
		printf("Failed to load resource!\nSee log for more details.\n");
	}
}

void populateWorld() {
	new Instruction();
}