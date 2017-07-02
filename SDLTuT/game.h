#pragma once
#include "animated_sprite.h"
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

	AnimatedSprite _player;
};