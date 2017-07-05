#pragma once

#include "animated_sprite.h"
#include "globals.h"
class Graphics;


class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, float x, float y);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	/* move by -dx*/
	void moveLeft();
	/*move by dx*/
	void moveRight();
	/*stops moving the player*/
	void stopMoving();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();
private:
	/*change in position*/
	float _dx, _dy;

	Direction _facing;
};