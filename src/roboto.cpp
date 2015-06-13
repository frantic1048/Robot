#include <emscripten/bind.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define Pi 3.14159265

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
    ;
}


class DirectedPoint: public Point
{
public:
  DirectedPoint(double x,double y,double theta)
    :Point(x,y)
    ,theta(theta)
    {}

  double getTheta () const { return theta; }
  void setTheta (double theta_) { theta = theta_; }

  double r () const {
    return std::sqrt(x*x + y*y);
  }

  void forward () {
    double fr = r() + 1.0;
    x = fr * std::cos(theta);
    y = fr * std::sin(theta);
  }
private:
  double theta;// in radian
};

// Directed Point constructor binding
EMSCRIPTEN_BINDINGS(directed_point) {
  class_<DirectedPoint>("DirectedPoint")
    .constructor<double, double, double>()
    .property("x", &Point::getX, &Point::setX)
    .property("y", &Point::getY, &Point::setY)
    .property("r", &DirectedPoint::r)
    .property("theta", &DirectedPoint::getTheta, &DirectedPoint::setTheta)
    .function("forward", &DirectedPoint::forward)
    ;
}
