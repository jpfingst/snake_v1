#include "mouse.h"

Mouse::Mouse(float x, float y)
{
  mouse_pic.loadFromFile("mouse_small.png");
  
  box.x = x;
  box.y = y;
  box.w = mouse_pic.getWidth();
  box.h = mouse_pic.getHeight();
}

Mouse::~Mouse()
{
  mouse_pic.free();
}

void Mouse::render()
{
  float angle = 0;
  mouse_pic.render(box.x, box.y, NULL, angle, NULL);
}

SDL_Rect Mouse::get_box()
{
  return box;
}
