#pragma once

#include "sprite.h"
#include "player.h"
class Graphics;

const int SPRITEWIDTH = 8;
class HUD {
public:
	HUD();
	HUD(Graphics &graphics, Player &player);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
private:
	Player _player;

	Sprite _healthBarSprite;
	Sprite _healthNumber1;
};