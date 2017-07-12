#include "player.h"
#include "graphics.h"
#include "slope.h"

/*could also include const string for player sprite*/
namespace player_constants {
	const int TIME_TO_UPDATE = 100;
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
	const float PLAYERHEIGHT = 16;
}
Player::Player() {}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "content/sprites/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, player_constants::TIME_TO_UPDATE),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false),
	_lookingDown(false),
	_lookingUp(false),
	_maxHealth(3),
	_currentHealth(2)
	{
	graphics.loadImage("content/sprites/MyChar.png");
	this->setupAnimations();
	this->playAnimation("RunRight");
	
	}

void Player::setupAnimations() {
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {

}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
}
void Player::moveLeft() {
	if (this->_lookingDown && this->_grounded) {
		return;
	}
	this->_dx = -player_constants::WALK_SPEED;
	if (!this->_lookingUp){
		this->playAnimation("RunLeft");
	}
	this->_facing = LEFT;
}

void Player::moveRight() {
	if (this->_lookingDown && this->_grounded) {
		return;
	}
	this->_dx = player_constants::WALK_SPEED;
	if (!this->_lookingUp) {
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	if (!this->_lookingUp && !this->_lookingDown) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
	}
}

void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}

}

void Player::lookUp() {
	this->_lookingUp = true;
	if (this->_dx == 0) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp() {
	this->_lookingUp = false;
}

void Player::lookDown() {
	if (this->_dx == 0) {
		this->_lookingDown = true;
		if (this->_grounded) {
			//we need to interact here
			this->playAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
		}
		else {
			this->playAnimation(this->_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
		}
	}
}

void Player::stopLookingDown() {
	this->_lookingDown = false;
}

//void handlTileCollisions
//Handles collisions with all tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	//Figure out what dies the colllision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0;
				this->_y = others.at(i).getBottom() + 1;
				if (this->_grounded) {
					this->_dx = 0;
					this->_x -= this->_facing == RIGHT ? 1 : -1;
				}
				break;
			case sides::BOTTOM:
				
				this->_y = others.at(i).getTop()- this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				
				//dx = 0?
				break;
			case sides::RIGHT:
				
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;

			}
		}

	}

}

//Handles all slopes the plaer is colliding with
void Player::handleSlopeCollisions(std::vector<Slope> &others) {
	for (int i = 0; i < others.size(); i++) {
		//calculate where on the slope the player's bottom center is touching
		//y = mx+b
		//First calculate "b" (slope intercept) using one of the points (b = y -mx)
		int b = (others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));

		//now get player's center x 
		int centerX = this->_boundingBox.getCenterX();
		//now pass that x into the equation to get the correct y heigh the player should be at
		int newY = (others.at(i).getSlope() * centerX) + b - 8;

		if (this->_grounded) {
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true; 
		}
	}

}

void Player::update(float elapsedTime) {
	//applying gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		this->_dy += player_constants::GRAVITY * elapsedTime;

	}
	


	this->_x += this->_dx * elapsedTime;
	this->_y += this->_dy * elapsedTime;
	

	AnimatedSprite::update(elapsedTime);
	
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}