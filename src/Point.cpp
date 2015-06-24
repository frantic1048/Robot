#ifndef __ROBOT_POINT_INCLUDED__
#define __ROBOT_POINT_INCLUDED__

#include <cstdlib>
#include <cmath>

#include <emscripten/bind.h>
using namespace emscripten;

class Point
{
public:
  Point (double x = 0.0,
         double y = 0.0)
    :x(x)
    ,y(y)
  {}

  double getX () const { return x; }
  void setX (double x_) { x = x_; }

  double getY () const { return y; }
  void setY (double y_) { y = y_; }

  double distanceTo (Point b) {
    return std::sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y));
  }

  double angleTo (Point b) {
    return std::atan2(b.y - y,b.x - x);
  }

  void operator () (double _x,double _y) {
    x = _x;
    y = _y;
  }
protected:
  double x;
  double y;
};

// Point constructor binding
EMSCRIPTEN_BINDINGS(point) {
  class_<Point>("Point")
    .constructor<double, double>()
    .property("x",&Point::getX, &Point::setX)
    .property("y",&Point::getY, &Point::setY)
    .function("distanceTo",&Point::distanceTo)
    ;
}

#endif
