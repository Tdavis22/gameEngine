#pragma once
#include "player.h"
#include "level.h"
#include "hud.h"
class Graphics;
/*
Holds all info for our game loop

*/

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics &graphics);
	void update(float elapsedTime);/*eslapsed time = delta*/

	Player _player;
	Level _level;
	HUD _hud;	
};