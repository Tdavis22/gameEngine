#pragma once

#include <map>
#include <string>
/*
Holds all information dealing with graphics for the game
*/
struct SDL_Window;
struct SDL_Renderer;


class Graphics {
public:
	Graphics();
	~Graphics();

	/*SDL_Surface* loadImage
	loads an image into the spriSheets map if it doesn't already exist
	returns the image from the map regardless of weather or not it was just loaded
	*/
	SDL_Surface* loadImage(const std::string &filePath);

	/*void blitSurface
	Draws SDL textures to a certain part of the screen renderer
	Source->sprite sheet
	sourceRectangle -> which sprite on sprite sheet
	destinationRectangle -> which square on screen
	*/
	void blitSurface(SDL_Texture * texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
	
	/*void flip
	Renders everything to the screen
	Puts Renders to the screen
	*/
	void flip();

	/*void clear
	Clears the screen
	Should be cleared at beggining of each drawing cycle
	*/
	void clear();

	/*SDL_Renderer* getRenderer
	returns the renderer
	*/

	SDL_Renderer* getRenderer() const;
private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;

	/*hold all of the loaded spritesheets*/
	std::map<std::string, SDL_Surface*> _spriteSheets;
	
};