#include "link.h"

Link::Link()
{
  x = 0;
  y = 0;
  type = HEAD;
}

Link::Link(double x, double y, Curve_Type type)
{
  this->x = x;
  this->y = y;
  this->type = type;
}

Link::~Link()
{

}

double Link::get_x()
{
  return this->x;
}

double Link::get_y()
{
  return this->y;
}

Curve_Type Link::get_type()
{
  return this->type;
}

void Link::set_x_y(double x, double y)
{
  this->x = x;
  this->y = y;
}

void Link::set_type(Curve_Type type)
{
  this->type = type;
}
