#include "level.h"
#include "graphics.h"
#include "globals.h"

#include "tinyxml2.h"


#include <SDL.h>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
	_mapName(mapName),
	_spawnPoint(spawnPoint),
	_size(Vector2(0, 0))
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {

}


void Level::loadMap(std::string mapName, Graphics&graphics) {
	//TEMPORARY CODE TO LOAD THE BACKGROUND
	//this->_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("content/backgrounds/bkBlue.png"));
	//this->_size = Vector2(1280, 960);

	//Parse the .tmx file
	//All of TinyXML writtin in C. Uses Char[] instead of String
	XMLDocument doc;
	std::stringstream ss;
	ss << "content/maps/" << mapName << ".tmx"; //Pass in map1, we get content/maps/map1.tmx
	doc.LoadFile(ss.str().c_str()); //String stream to string to c String(char[])

	XMLElement * mapNode = doc.FirstChildElement("map");

	//get the width and the hiegh of the whole map and store it in size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	//Get the width and the height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	//loading the first tilesets
	XMLElement * pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		//search through linked list
		while (pTileset) {

		//Parses through xml of Map to get tile sheet and PNG used for tilesheet
			int firstgid;
			//const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			const char* source = pTileset->Attribute("source");
			std::stringstream ss;
			ss << "content/maps/" <<source; //find tileset xml file
			pTileset->QueryIntAttribute("firstgid", &firstgid);

			XMLDocument tiledoc;
			tiledoc.LoadFile(ss.str().c_str()); //String stream to string to c String(char[])
	
			XMLElement * imageNode = tiledoc.FirstChildElement("tileset")->FirstChildElement("image");
			const char* path = imageNode->Attribute("source");
			std::stringstream ss2;
			ss2 << "content/tilesets/" << path; //find tileset xml file
			SDL_Texture * tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss2.str()));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}
	XMLElement * player = mapNode->FirstChildElement("layer");
	if (player != NULL) {
		while (player) {
			//load the data element
			XMLElement* pData = player->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					//loadiong the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//Building each individual tile
							//If gid is 0 no tile is drawn
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							//Get the tileset for this specific gid(when using multiple tilesets)
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									tls = this->_tilesets.at(i);
									break;
									}
                            }
							//unInitialized tile set
							if (tls.FirstGid == -1) {
								//No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the position of the tile in the level
							/*example of 2nd tile for next two lines of code
							Counter = 1
							Width = 20 (size of map)
							xx will be set to 16px where it should start
							*/
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy);

							//Calculate the position of the tile in the tileset
							int tilesetWidth, tilesetHeight;
							SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
							int tsxx = gid % (tilesetWidth / tileWidth) -1;//Tileset xx post for a tile
							tsxx *= tileWidth; //will bring us to correct column

							int tsyy = 0;
							tsyy = (gid / (tilesetWidth / tileWidth));
							tsyy = tsyy * tileHeight;
							Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

							//Build the actual tile and add it to the level's tile list
							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight),
								finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}
			player = player->NextSiblingElement("layer");
		}
	}
}

void Level::update(int elapsedTime) {

}

void Level::draw(Graphics &graphics) {
	//Draw the background
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
	
	/*SDL_Rect sourceRect = {0, 0, 64, 64};
	SDL_Rect destRect;/*will be drawn to every square

	for (int x = 0; x < this->_size.x; x+= 64) {
		for (int y = 0; y < this->_size.y; y+= 64) {
			destRect.x = x * globals::SPRITE_SCALE;
			destRect.y = y * globals::SPRITE_SCALE;
			destRect.w = 64 * globals::SPRITE_SCALE;
			destRect.h = 64 * globals::SPRITE_SCALE;
			graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
		}

	}*/
}