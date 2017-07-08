#pragma once


namespace globals {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	const int SPRITE_SCALE = 2;
}
namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};
	const inline Side getOppositeSide(Side side) {
		//returns the opposite side. Parameter top: Return = bottom
		//basically a few if else statements with fancy turnary syntax
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DWON
};

struct Vector2 {
	int x, y; /* be default public*/
	Vector2() :
		x(0), y(0)
	{}

	Vector2(int x, int y):
	x(x), y(y)
	{}


	Vector2 zero() {
		return Vector2(0, 0);
	}
};