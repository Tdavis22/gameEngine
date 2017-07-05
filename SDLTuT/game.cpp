#include <SDL.h>

#include "game.h"
#include "graphics.h"
#include "input.h"


namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 5 * 1000 / FPS; //maximum time a frame is alowed to last
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	this -> gameLoop();
}
Game::~Game() {

}

void Game::gameLoop() {

	/*passed to every class for drawing*/
	Graphics graphics;
	Input input;
	SDL_Event event;/*hold input every frame*/
    
	/*Sprites are 16x16
	setting player sprite to first image in player sprite
	*/
	this->_player = Player(graphics, 100, 100);
	this->_level = Level("map 1", Vector2(100, 100), graphics);
	
	int LAST_UPDATE_TIME = (int)SDL_GetTicks(); //ms after game init
	//Start game loop
	while (true) {
		input.beginNewFrame();
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) { //key.repeat = holding down a key
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			if (event.type == SDL_QUIT) {
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			return;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_LEFT)) {
			this->_player.moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_player.moveRight();
		}

		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_player.stopMoving();
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		//passes the smaller of elapsed time or max frame time. so if computer is slow the game still keeps updating physics
		this->update((ELAPSED_TIME_MS < MAX_FRAME_TIME ? ELAPSED_TIME_MS : MAX_FRAME_TIME)); //can't go higher than 50 frames per second. Physics based on 50fps
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this -> draw(graphics);
	}
}

void Game::draw(Graphics & graphics) {
	graphics.clear();
	this->_level.draw(graphics);
	this->_player.draw(graphics);

	graphics.flip();
}

void Game::update(float elapsedtime) {
	this->_player.update(elapsedtime); /*should update all entities*/
	this->_level.update(elapsedtime);
}