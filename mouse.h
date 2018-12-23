#ifndef MOUSE_H
#define MOUSE_H

#include <SDL.h>
#include <stdio.h>
#include "texture.h"

class Mouse
{
 public:
  Mouse(float x, float y);
  ~Mouse();

  void render();
  SDL_Rect get_box();
  
 private:
  SDL_Rect box;
  Texture mouse_pic;
};

#endif
