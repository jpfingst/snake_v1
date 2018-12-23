#ifndef TILE_H
#define TILE_H

#include <SDL.h>
#include "texture.h"

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 80;
const int TOTAL_TILES_SPRITE = 12;

extern Texture gTileTexture;
extern SDL_Rect gTileClips[TOTAL_TILES_SPRITE];

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

class Tile
{
public:
  Tile(float x, float y, int tileType);
  ~Tile();

  void render();
  int getType();
  SDL_Rect getBox();
  
private:
  int type;
  SDL_Rect box;
};

#endif
