#ifndef SCORE_H
#define SCORE_H

#include "texture.h"

class Score
{
 public:
  Score();
  ~Score();

  void increment();
  void render();

 private:
  int score_counter;
  Texture mini_mouse_texture;
  Texture score_texture;
};

#endif
