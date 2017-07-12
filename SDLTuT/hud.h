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
	Sprite _healthNumber1;//Right most digit for health. 
					      //In case of 5 health it is 5. In case of 12 health it is 2
	Sprite _currentHealthBar;
};