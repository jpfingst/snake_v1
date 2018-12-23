#include "tile.h"

Tile::Tile(float x, float y, int tileType)
{
  box.x = x;
  box.y = y;
  box.w = TILE_WIDTH;
  box.h = TILE_HEIGHT;

  this->type = tileType;
}

Tile::~Tile()
{

}

void Tile::render()
{
  gTileTexture.render( box.x, box.y, &gTileClips[ type ] );
}

int Tile::getType()
{
  return type;
}

SDL_Rect Tile::getBox()
{
  return box;
}
