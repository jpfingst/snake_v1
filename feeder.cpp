#include "feeder.h"

Feeder::Feeder()
{
  clear_list();
  srand(time(NULL));
}

Feeder::~Feeder()
{
  clear_list();
}

Mouse* Feeder::dropMouse(Tile *tiles[], Snake* the_snake)
{
  /////////////////////////////
  // Find viable coordinates //
  /////////////////////////////

  
  // Go through all tiles and check if they are free
  bool free_tile = true;
  for(int i=0; i<TOTAL_TILES; i++)
    {
      free_tile = true;

      // Is it a wall type tile?
      if((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
	{
	  free_tile = false;
	}

      // Is the snake already on the tile?
      if(the_snake->check_Collision(tiles[i]->getBox()))
	{
	  free_tile = false;
	}

      // When tile is free put the number in a list
      if(free_tile)
	{
	  tile_index_list.push_back(i);
	}

    }
  
  // Generate a random number from 1 to the length of the list
  int list_index = rand()%tile_index_list.size();
  for(int i = 0; i < list_index; i++)
    {
      tile_index_list.pop_front();
    }
  int rand_tile_index = tile_index_list.front();
  SDL_Rect random_tile_box = tiles[rand_tile_index]->getBox();
  clear_list();
  
  // Create the new mouse
  Mouse* new_mouse = new(Mouse)(random_tile_box.x, random_tile_box.y);
  return new_mouse;
}

void Feeder::clear_list()
{
  while(tile_index_list.size())
    {
      tile_index_list.pop_back();
    }
}
