#include <stdio.h>

#include "LogManager.h"
#include "ResourceManager.h"

#define SPRITE_FOLDER "unit_tests/sprites/"
#define SOUND_FOLDER  "unit_tests/sounds/"
#define MUSIC_FOLDER  "unit_tests/sounds/"

int main() {

	int failed_tests = 0;

	df::ResourceManager &rm = df::ResourceManager::getInstance();
	df::LogManager::getInstance().startUp();

	if( rm.startUp() ) {
		printf("ResourceManager failed to start up!\n");
		failed_tests++;
	}
	if(0 != rm.loadSprite(SPRITE_FOLDER "bullet-spr.txt", "bullet")) {
		printf("ResourceManager: Problem loading sprite!\n");
		failed_tests++;
	}
	if(0 != rm.loadSprite(SPRITE_FOLDER "gameover-spr.txt", "gameover")) {
		printf("ResourceManager: Problem loading sprite!\n");
		failed_tests++;
	}

	if(-1 != rm.loadSprite(SPRITE_FOLDER "gameover-spr.txt", "gameover")) {
		printf("ResourceManager: Allowed loading of duplicate sprite!\n");
		failed_tests++;
	}
	if(rm.getSprite("bullet") == NULL ||
		rm.getSprite("bullet")->getFrameCount() != 2) {
		printf("ResourceManager: Unable to get sprite!\n");
		failed_tests++;
	}
	if(0 != rm.unloadSprite("bullet") ||
		rm.getSprite("bullet") != NULL) {
		printf("ResourceManager: Unable to unload sprite!\n");
		failed_tests++;
	}

	// Sound tests
	if(0 != rm.loadSound(SOUND_FOLDER "fire.wav", "fire")) {
		printf("ResourceManager: Problem loading sound!\n");
		failed_tests++;
	}
	if(0 != rm.loadSound(SOUND_FOLDER "game-over.wav", "gameover")) {
		printf("ResourceManager: Problem loading a second sound!\n");
		failed_tests++;
	}

	if(-1 != rm.loadSound(SOUND_FOLDER "game-over.wav", "gameover")) {
		printf("ResourceManager: Allowed loading of duplicate sound!\n");
		failed_tests++;
	}
	if(rm.getSound("fire") == NULL ||
		rm.getSound("fire")->getLabel() != "fire") {
		printf("ResourceManager: Unable to get sound!\n");
		failed_tests++;
	}
	if(0 != rm.unloadSound("fire") ||
		rm.getSound("fire") != NULL) {
		printf("ResourceManager: Unable to unload sound!\n");
		failed_tests++;
	}

	// Music tests
	if(0 != rm.loadMusic(MUSIC_FOLDER "fire.wav", "fire")) {
		printf("ResourceManager: Problem loading music!\n");
		failed_tests++;
	}
	if(0 != rm.loadMusic(MUSIC_FOLDER "game-over.wav", "gameover")) {
		printf("ResourceManager: Problem loading a second music!\n");
		failed_tests++;
	}
	if(-1 != rm.loadMusic(MUSIC_FOLDER "game-over.wav", "gameover")) {
		printf("ResourceManager: Allowed loading of duplicate music!\n");
		failed_tests++;
	}
	if( rm.getMusic("fire") == NULL ||
		rm.getMusic("fire")->getLabel() != "fire") {

		printf("ResourceManager: Unable to get music!\n");
		failed_tests++;
	}
	if(0 != rm.unloadMusic("fire") ||
		rm.getMusic("fire") != NULL) {
		printf("ResourceManager: Unable to unload music!\n");
		failed_tests++;
	}


	rm.shutDown();

	df::LogManager::getInstance().shutDown();

	return failed_tests;
}