#ifndef __ROBOTO_VECTOR_INCLUDED__
#define __ROBOTO_VECTOR_INCLUDED__

#include <cstdlib>
#include <cmath>

#include <emscripten/bind.h>
using namespace emscripten;

class Point;


class Vector: public Point
{
private:
  double theta;// in radian

public:
  Vector ()
    :Point(0, 0)
    ,theta(0)
    {}

  Vector(double x,double y,double theta)
    :Point(x,y)
    ,theta(theta)
    {}

  double getTheta () const { return theta; }
  void setTheta (double theta_) { theta = theta_; }

  double r () const {
    return std::sqrt(x*x + y*y);
  }

  void forward () {
    x = x + 1.0 * std::cos(theta);
    y = y + 1.0 * std::sin(theta);
  }

  void operator () (double _x, double _y, double _theta) {
    x = _x;
    y = _y;
    theta = _theta;
  }
};

// Vector constructor binding
EMSCRIPTEN_BINDINGS(my_vector) {
  class_<Vector>("Vector")
    .constructor<double, double, double>()
    .property("x", &Point::getX, &Point::setX)
    .property("y", &Point::getY, &Point::setY)
    .property("r", &Vector::r)
    .property("theta", &Vector::getTheta, &Vector::setTheta)
    .function("forward", &Vector::forward)
    ;
}

#endif
