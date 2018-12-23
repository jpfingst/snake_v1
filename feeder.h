#ifndef FEEDER_H
#define FEEDER_H

#include "mouse.h"
#include "snake.h"
#include "tile.h"
#include <list>
#include <time.h>
//#include <stdlib.h>

class Feeder
{
 public:
  Feeder();
  ~Feeder();

  Mouse* dropMouse(Tile *tiles[], Snake* the_snake);

 private:
  std::list<int> tile_index_list;
  void clear_list();
  
};

#endif
