#include <emscripten/bind.h>
#include <cstdlib>
#include <cmath>

using namespace emscripten;

class Vector: public Point
{
public:
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
    double fr = r() + 1.0;
    x = fr * std::cos(theta);
    y = fr * std::sin(theta);
  }
private:
  double theta;// in radian
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
