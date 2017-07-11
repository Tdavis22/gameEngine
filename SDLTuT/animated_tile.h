#pragma once
#include "tile.h"
#include <vector>
#include "globals.h"

class AnimatedTile : public Tile {
public:
	AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture * tileset,
		Vector2 size, Vector2 position);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
protected:
	int _amountOfTime = 0;
	bool _notDone = false;

private:
	std::vector<Vector2> _tilesetPositions;
	int _tileToDraw;
	int _duration;
};

struct AnimatedTileInfo {
	int TilesetsFirstGid;
	int StartTileId;
	int Duration;
	std::vector<int> TileIds;

};