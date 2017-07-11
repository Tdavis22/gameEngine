#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "utils.h"
#include "slope.h"

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
			const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			//const char* source = pTileset->Attribute("source");
			std::stringstream ss;
			ss << "content/maps/" <<source; //find tileset xml file
			pTileset->QueryIntAttribute("firstgid", &firstgid);

			//XMLDocument tiledoc;
			//tiledoc.LoadFile(ss.str().c_str()); //String stream to string to c String(char[])
	
			//XMLElement * imageNode = tiledoc.FirstChildElement("tileset")->FirstChildElement("image");
			//const char* path = imageNode->Attribute("source");
			//std::stringstream ss2;
			//ss2 << "content/tilesets/" << path; //find tileset xml file
			SDL_Texture * tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));//was ss2
			this->_tilesets.push_back(Tileset(tex, firstgid));

			//Get all of the animations for that tileset
			XMLElement * pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL) {
				while (pTileA) {
					//create list of animated tile infos
					AnimatedTileInfo ati;
					ati.StartTileId = pTileA->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;
					XMLElement * pAnimation = pTileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						while (pAnimation) {
							XMLElement * pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration"); //will be run a lot for code simplicy but inefficent
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->_animatedTileInfos.push_back(ati);
					pTileA = pTileA->NextSiblingElement("tile");
				}
			}






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
							int closest = 0;
							//loop through all tilesets
							for (int i = 0; i < this->_tilesets.size(); ++i) {
								if (this->_tilesets[i].FirstGid <= gid) {
									if (this->_tilesets[i].FirstGid > closest) {
										closest = this->_tilesets[i].FirstGid;
										tls = this->_tilesets.at(i);
									}
									
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
							
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < this->_animatedTileInfos.size(); ++i) {
								if (this->_animatedTileInfos.at(i).StartTileId == gid) {
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile) {
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); ++i) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i), tileWidth, tileHeight));
								}
									AnimatedTile tile(tilesetPositions, ati.Duration, tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
									this->_animatedTileList.push_back(tile);
									
								
							}
							else {
								//Build the actual tile and add it to the level's tile list
								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight),
									finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}
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

	//Parse out the collisions
	XMLElement * pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "collisions") {
				XMLElement * pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");

						this->_collsionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
						    std::ceil(y) * globals::SPRITE_SCALE,
						    std::ceil(width) * globals::SPRITE_SCALE,
						    std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			//other objectgroups go here with an else if (ss.str() == "something")
			else if (ss.str() == "slopes") {
				//In the xml all slops are objects with x and y
				//Poly lines are point offsets from the x and y
				XMLElement * pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));
						XMLElement * pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');
							//Now we have each of the pairs. Loop through the list of pairs
							// and split them in Vector2s and then store them in our points vector
							for (int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));

							}
						}
						//going in groups of 2 points
						for (int i = 0; i < points.size(); i += 2) {
							this->_slopes.push_back(Slope(
								//I think the turnary is not necessary
								Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
								(p1.y + points.at(i < 2 ? i : i - 1).y) * globals::SPRITE_SCALE),

								Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
								(p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE)
							));
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}




			}
			
			
			
			else if (ss.str() == "spawn points") {
				XMLElement * pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

void Level::update(int elapsedTime) {
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).update(elapsedTime);
	}
}

void Level::draw(Graphics &graphics) {
	//Draw the background
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		
		this->_animatedTileList.at(i).draw(graphics);
	}

}

//Returns a vector of every rectangle that is colliding
std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
	std::vector<Rectangle> others;
	for (int i = 0; i< this->_collsionRects.size(); ++i) {
		if (this->_collsionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collsionRects.at(i));
		}
	}
	return others;
}
std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
	std::vector<Slope> others;
	for (int i = 0; i < this->_slopes.size(); i++) {
		if (this->_slopes.at(i).collidesWith(other)) {
			others.push_back(this->_slopes.at(i));
		}
	}
	return others;
}
const Vector2 Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = (gid -1) % (tilesetWidth / tileWidth);//Tileset xx post for a tile
	tsxx *= tileWidth; //will bring us to correct column

	int tsyy = 0;
	tsyy = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	tsyy = tsyy * tileHeight;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

	return finalTilesetPosition;
}