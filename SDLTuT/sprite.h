#pragma once

/*Sprite class
Holds all information for individual sprites
*/
#include <sdl.h>
class Graphics;
#include <string>
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
protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;
private:
	float _x, _y;
};