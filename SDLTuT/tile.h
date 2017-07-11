#pragma once

#include "globals.h"
struct SDL_Texture;
class Graphics;

class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPostion, Vector2 position);

	void update(int elapsedTime);
	void draw(Graphics &graphics);
protected:
	SDL_Texture* _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition; //location on the tileset
	Vector2 _position; //location on map
};