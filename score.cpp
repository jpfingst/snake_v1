#include "score.h"

Score::Score()
{
  score_counter = 0;
  mini_mouse_texture.loadFromFile("mouse_small.png");

  SDL_Color white = {0xFF, 0xFF, 0xFF};
  score_texture.loadFromRenderedText("0", white);
}

Score::~Score()
{
  mini_mouse_texture.free();
}

void Score::increment()
{
  score_counter++;
}

void Score::render()
{
  SDL_Color white = {0xFF, 0xFF, 0xFF};
  std::string score = std::to_string(score_counter);
  score_texture.loadFromRenderedText(score, white);
  score_texture.render(20, 25);
  
  mini_mouse_texture.render(70, 15);
}
