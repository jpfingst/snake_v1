#include "snake.h"

Snake::Snake(double x, double y, double speed)
{
  double snake_length0 = 100;
  Link head(x, y, HEAD);
  Link tail(x - snake_length0, y, TAIL);
  curves.push_front(head);
  curves.push_back(tail);

  snakeHead_texture.loadFromFile("boa-head.png");

  direct = RIGHT;
  snake_width = 20;
  this->speed = speed; // Pixel per frame

  gMoveSound = Mix_LoadWAV("shh-shh.wav");
  gEatSound = Mix_LoadWAV("rrrou-1.wav");
  
}

Snake::~Snake()
{
  // Free the texture of the snake head
  snakeHead_texture.free();

  // Free the sound resources 
  Mix_FreeChunk(gMoveSound);
  Mix_FreeChunk(gEatSound);
  gMoveSound = NULL;
  gEatSound = NULL;
  
  // Delete the link list
  while(curves.size())
    {
      curves.pop_front();
    }
}

void Snake::handle_Event(SDL_Event& e)
{
  direct_old = this->direct;
  
  // If a key was pressed
  if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
      
      switch(e.key.keysym.sym)
	{
	case SDLK_UP:
	  if(this->direct != DOWN)
	    {
	      this->direct = UP;
	      Mix_PlayChannel(-1, gMoveSound, 0);
	    }
	  break;
	case SDLK_DOWN:
	  if(this->direct != UP)
	    {
	      this->direct = DOWN;
	      Mix_PlayChannel(-1, gMoveSound, 0);
	    }
	  break;
	case SDLK_LEFT:
	  if(this->direct != RIGHT)
	    {
	      this->direct = LEFT;
	      Mix_PlayChannel(-1, gMoveSound, 0);
	    }
	  break;
	case SDLK_RIGHT:
	  if(this->direct != LEFT)
	    {
	      this->direct = RIGHT;
	      Mix_PlayChannel(-1, gMoveSound, 0);
	    }
	  break;
	}
    }
}

void Snake::advance()
{
  Link head_link = curves.front();
  curves.pop_front();
  double x = head_link.get_x(); 
  double y = head_link.get_y();
  
  // Check if a new curve has to be added
  if(direct_old != direct)
    {
      // Create new link
      Curve_Type type = determine_curve_type(direct_old, direct);
      Link new_curve(x, y, type);

      // Add the new link just after the head
      curves.push_front(new_curve);
    }
  direct_old = direct;
  
  // Advance the head
  switch(this->direct)
    {
    case UP:
      y -= speed;
      break;
    case DOWN:
      y += speed;
      break;
    case LEFT:
      x -= speed;
      break;
    case RIGHT:
      x += speed;
      break;
    }

  // Add the head again
  head_link.set_x_y(x, y);
  curves.push_front(head_link);

  // Check if the tail should be advanced
  double dist_remaining = speed;
  if(prolong_dist > speed)
    {
      dist_remaining -= speed;
      prolong_dist -= speed;
    }
  else
    {
      dist_remaining -= prolong_dist;
      prolong_dist = 0;
    }
  
  // Advance the tail and delete curves if necessary
  double dist_x = 0, dist_y = 0, dist_tail_curve = 0;
  Link tail = curves.back();
  curves.pop_back();
  Link curve_end;
  bool overtake_curve = true;
  while(overtake_curve)
    {
      curve_end = curves.back();
      dist_x = tail.get_x() - curve_end.get_x();
      dist_y = tail.get_y() - curve_end.get_y();
      dist_tail_curve = sqrt( dist_x * dist_x + dist_y * dist_y);
      if(dist_tail_curve > dist_remaining)
	{
	  // Last curve is not going to be deleted
	  overtake_curve = false;
	}
      else
	{
	  // Last curve has to be deleted
	  tail = curve_end;
	  tail.set_type(TAIL);
	  curves.pop_back();
	  dist_remaining -= dist_tail_curve;
	}
    }

  // Move the tail in direction of the last curve and add it again
  if(abs(dist_x) > abs(dist_y))
    {
      // Move in x direction
      if(dist_x > 0)
	{
	  tail.set_x_y(tail.get_x() - dist_remaining, tail.get_y());
	}
      else
	{
	  tail.set_x_y(tail.get_x() + dist_remaining, tail.get_y());
	}
    }
  else
    {
      // Move in y direction
      if(dist_y > 0)
	{
	  tail.set_x_y(tail.get_x(), tail.get_y() - dist_remaining);
	}
      else
	{
	  tail.set_x_y(tail.get_x(), tail.get_y() + dist_remaining);
	}
    }
  curves.push_back(tail);
  
  
}

void Snake::eatMouse(Mouse* mouse)
{
  Mix_PlayChannel(-1, gEatSound, 0);
  delete(mouse);
  mouse = NULL;
}

void Snake::render()
{
  // Render each link first
  std::list<Link>::iterator it=begin(curves);
  SDL_Rect link_rect;
  double x_link1 = 0, x_link2 = 0, y_link1 = 0, y_link2 = 0;
  for(it = curves.begin(); it != curves.end(); it++)
    {
      if(it->get_type() != TAIL)
	{
	  x_link1 = it->get_x();
	  y_link1 = it->get_y();
	  it++;
	  x_link2 = it->get_x();
	  y_link2 = it->get_y();
	  it--;
	  link_rect = get_box_from_coord(x_link1, y_link1, x_link2, y_link2, snake_width);
	  SDL_SetRenderDrawColor( gRenderer, 204, 200, 65, 0xFF );        
	  SDL_RenderFillRect( gRenderer, &link_rect );
	}
    }
  
  // Render snake head on top
  SDL_Rect head_box = get_snake_head_box();
  double angle=0.0;
  switch(direct)
    {
    case UP: angle = 90.0; break;
    case DOWN: angle = 270.0; break;
    case LEFT: angle = 0.0; break;
    case RIGHT: angle = 180.0; break;
    }
  
  snakeHead_texture.render(head_box.x, head_box.y, NULL, angle, NULL);
}

SDL_Rect Snake::get_snake_head_box()
{
  // Create the box that contains the snake head
  SDL_Rect snake_head_box;
  Link head_link = curves.front();
  float x_neck = head_link.get_x(); 
  float y_neck = head_link.get_y();
  float pic_width = snakeHead_texture.getWidth();
  float pic_height = snakeHead_texture.getHeight();
  
  switch(direct)
    {
    case UP:
      snake_head_box.w = pic_height;
      snake_head_box.h = pic_width;
      snake_head_box.x = x_neck - pic_width / 2;
      snake_head_box.y = y_neck - pic_height / 2;
      break;
    case DOWN:
      snake_head_box.w = pic_height;
      snake_head_box.h = pic_width;
      snake_head_box.x = x_neck - pic_width / 2;
      snake_head_box.y = y_neck - pic_height / 2; 
      break;
    case LEFT:
      snake_head_box.w = pic_width;
      snake_head_box.h = pic_height;
      snake_head_box.x = x_neck - pic_width / 2;
      snake_head_box.y = y_neck - pic_height / 2;
      break;
    case RIGHT:
      snake_head_box.w = pic_width;
      snake_head_box.h = pic_height;
      snake_head_box.x = x_neck - pic_width / 2;
      snake_head_box.y = y_neck - pic_height / 2;
      break;
    }
  
  return snake_head_box;
}

bool Snake::check_Collision(SDL_Rect box_ext)
{
  bool collision = false;

  // Check the head for a collision
  if(::check_Collision(box_ext, this->get_snake_head_box()))
    {
      collision = true;
    }

  // Check each link/limp for a collision
  std::list<Link>::iterator it1 = curves.begin();
  std::list<Link>::iterator it2 = curves.begin();
  it2++;
  for(; it2 != curves.end(); it2++)
    {
      SDL_Rect link_box = get_box_from_coord(it1->get_x(), it1->get_y(), it2->get_x(), it2->get_y(), this->snake_width);
      if(::check_Collision(link_box, box_ext))
	{
	  collision = true;
	}
      it1++;
    }
  
  return collision;
}

bool Snake::check_Self_Collision()
{
  bool collision = false;

  if(curves.size() < 5)
    {
      // Collision is not possible
    }
  else
    {
      // Get iterator and place them at the second link
      std::list<Link>::iterator it1 = curves.begin();
      std::list<Link>::iterator it2 = curves.begin();
      it1++;  it1++;  it1++;
      it2++;  it2++;  it2++;  it2++;

      int counter = 1;
      // Go through all remaining links and check a collision with snake's head
      for(; it2 != curves.end(); it2++)
	{
	  // Get snakes link box
	  SDL_Rect link_box = get_box_from_coord(it1->get_x(), it1->get_y(), it2->get_x(), it2->get_y(), this->snake_width);
	  if(::check_Collision(link_box, this->get_snake_head_box()))
	    {
	      collision = true;
	    }
	  it1++;
	  counter++;
	}
    }
  // dEBUG
  return collision;
}

Curve_Type Snake::determine_curve_type(Direction direct_old, Direction direct_new)
{
  Curve_Type type=HEAD;

  switch(direct_old)
    {
    case UP:
      if(direct_new == LEFT)
	{
	  type = UP_LEFT;
	}
      else
	{
	  type = UP_RIGHT;
	}
      break;

    case DOWN:
      if(direct_new == LEFT)
	{
	  type = DOWN_LEFT;
	}
      else
	{
	  type = DOWN_RIGHT;
	}
      break;

    case LEFT:
      if(direct_new == UP)
	{
	  type = UP_LEFT;
	}
      else
	{
	  type = DOWN_LEFT;
	}
      break;
      
    case RIGHT:
      if(direct_new == UP)
	{
	  type = UP_RIGHT;
	}
      else
	{
	  type = DOWN_RIGHT;
	}
      break;
    }
  
  return type;
}

SDL_Rect Snake::get_box_from_coord(double x1, double y1, double x2, double y2, double width)
{
  SDL_Rect box;
  double diff_x = abs(x1 - x2);
  double diff_y = abs(y1 - y2);

  if(diff_x > diff_y)
    {
      // A horizontal element
      if(x1 < x2)
	{
	  box.x = x1;
	}
      else
	{
	  box.x = x2;
	}
      box.y = y1 - width / 2;
      box.w = diff_x;
      box.h = width;
    }
  else
    {
      // A vertical element
      box.x = x1 - width / 2;
      if(y1 < y2)
	{
	  box.y = y1;
	}
      else
	{
	  box.y = y2;
	}
      box.w = width;
      box.h = diff_y;
    }
  return box;
}

void Snake::prolong(double length)
{
  prolong_dist += length;
}
