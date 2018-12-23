#ifndef LINK_H
#define LINK_H

enum Curve_Type
  {
    HEAD,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
    TAIL
  };

class Link
{
 public:
  Link();
  Link(double x, double y, Curve_Type type);
  ~Link();

  double get_x();
  double get_y();
  Curve_Type get_type();
  void set_x_y(double x, double y);
  void set_type(Curve_Type type);
  
 private:
  double x, y;
  Curve_Type type;
};

#endif
