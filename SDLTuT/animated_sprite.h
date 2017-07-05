#pragma once

/* parent class is sprite*/
#include <string.h>
#include <map>
#include <vector>
#include "sprite.h"
#include "globals.h"
class Graphics;

/* AnimatedSprite class
Holds logic for animating sprites

*/

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filePath,
		int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);


	/* void playAnimation
	Plays the animation provided if it's not already paying
	2nd parameter for looping
	*/
	void playAnimation(std::string animation, bool once = false); 
	

	/*void update
	Updates the animated sprite (timer)
	
	*/
	void update(int elapsedTime);

	/*void draw
	Draws the sprite to the screen
	*/
	void draw(Graphics &graphics, int x, int y);



protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;

	/*void addAnimation
	Adds an animation to the map of animations for the sprite
	*/

	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	/*void resetAnimations
	Resets all animations associated with this sprite
	*/
	void resetAnimations();

	/*void stopAnimations
	Stops the current animations*/
	void stopAnimations();

	/*void setVisable
	Changes the visibility of the animated sprite
	*/
	void setVisible(bool visible);

	/*void animationDone
	Logic that happens when an animation ends
	*/
	virtual void animationDone(std::string currentAnimation) = 0;

	/*void setupAnmations
	A required functions that sets up all animations for a sprite
     = 0 means pure virtual	
	*/

	virtual void setupAnimations() = 0;
private:
	/*vector of rectangles filled with animation frame/locations on sprite sheet*/
	std::map < std::string, std::vector<SDL_Rect> > _animations;

	/*Offset of where the sprites are located
	useful for larger sprites*/
	std::map<std::string, Vector2> _offsets;

	int _frameIndex;
	double _timeElapsed;
	bool _visable;
};