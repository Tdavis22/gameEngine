
#include <SDL.h>
#include "graphics.h"

/*
Holds all information dealing with graphics for the game
*/

Graphics::Graphics() {
    SDL_CreateWindowAndRenderer(640/*width*/, 480, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Cavestory");


}
Graphics::~Graphics() {
	SDL_DestroyWindow(this->_window);
}