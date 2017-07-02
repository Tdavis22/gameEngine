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
	Graphics graphics;
	Input input;
	SDL_Event event;/*hold input every frame*/
    
	/*Sprites are 16x16
	setting player sprite to first image in player sprite
	*/
	this->_player = AnimatedSprite(graphics, "content/sprites/MyChar.png", 0, 0, 16, 16, 100, 100, 100);
	this->_player.setupAnimations();
	this->_player.playAnimation("RunRight");
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
	this->_player.draw(graphics, 100, 100);

	graphics.flip();
}

void Game::update(float elapsedtime) {
	this->_player.update(elapsedtime); /*should update all entities*/
}