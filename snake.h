#ifndef SNAKE_H
#define SNAKE_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "texture.h"
#include "tile.h"
#include "mouse.h"
#include "link.h"
#include <list>

extern bool check_Collision(SDL_Rect a, SDL_Rect b);

enum Direction
  {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

class Snake
{
public:
  Snake(double x, double y, double speed_snake);
  ~Snake();

  void handle_Event(SDL_Event& e);
  void advance();
  void eatMouse(Mouse* mouse);
  void render();

  SDL_Rect get_snake_head_box();
  bool check_Collision(SDL_Rect box_ext);
  bool check_Self_Collision();
  void prolong(double length);
    
private:
  std::list<Link> curves;
  Direction direct, direct_old;
  double speed;
  double snake_width;
  double prolong_dist;
  Texture snakeHead_texture;
  Mix_Chunk* gMoveSound;
  Mix_Chunk* gEatSound;

  Curve_Type determine_curve_type(Direction direct_old, Direction direct_new);
  SDL_Rect get_box_from_coord(double x1, double y1, double x2, double y2, double width);
};

#endif
