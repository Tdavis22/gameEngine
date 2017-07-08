#pragma once

/*Sprite class
Holds all information for individual sprites
*/
#include <string>
#include <sdl.h>

#include "rectangle.h"
#include "globals.h"
class Graphics;


//#include "graphics.h"
class Sprite {
public: 
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY,
		int width, int height, float posX, float posY);
	virtual ~Sprite();

	/*virtual as seperate for still and animated sprite*/
	virtual void update();
	void draw(Graphics &graphics, int x, int y);

	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle &other) const;
protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;

	Rectangle _boundingBox;

	float _x, _y;
private:
	
};