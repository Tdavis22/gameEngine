#pragma once

#include "animated_sprite.h"
#include "globals.h"
#include "slope.h"
class Graphics;


class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	/* move by -dx*/
	void moveLeft();
	/*move by dx*/
	void moveRight();
	/*stops moving the player*/
	void stopMoving();

	void jump();

	//Animation changes
	void lookUp();
	void stopLookingUp();
	void lookDown();
	void stopLookingDown();

	

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();
	
	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);
	const float getX() const;
	const float getY() const;
private:
	/*change in position*/
	float _dx, _dy;

	Direction _facing;

	bool _grounded;
	bool _lookingUp;
	bool _lookingDown;
};