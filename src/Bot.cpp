#ifndef __ROBOT_BOT_INCLUDED__
#define __ROBOT_BOT_INCLUDED__

#include <cstdlib>
#include <cmath>

#include <emscripten/bind.h>
using namespace emscripten;

class Point;

class Bot: public Point
{
private:
  double theta;// theta (in radian) value in polar coordinate system

public:
  Bot ()
    :Point(0, 0)
    ,theta(0)
    {}

  Bot(double x,double y,double theta)
    :Point(x,y)
    ,theta(theta)
    {}

  double getTheta () const { return theta; }
  void setTheta (double theta_) { theta = theta_; }

  // radius value in polar coordinate system
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
EMSCRIPTEN_BINDINGS(bot) {
  class_<Bot>("Bot")
    .constructor<double, double, double>()
    .property("x", &Point::getX, &Point::setX)
    .property("y", &Point::getY, &Point::setY)
    .property("r", &Bot::r)
    .property("theta", &Bot::getTheta, &Bot::setTheta)
    .function("forward", &Bot::forward)
    ;
}

#endif
