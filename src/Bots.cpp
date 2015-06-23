#ifndef __ROBOTO_BOTS_INCLUDED__
#define __ROBOTO_BOTS_INCLUDED__

#include <cstdlib>
#include <cmath>

#include <emscripten/bind.h>
using namespace emscripten;

class Vector;
double getRandomNumberBetween(double lowerBound, double upperBound);

#define PI 3.141592653589793
#define BOT_VISION_RADUIS 30
#define BOT_INIT_RADIUS 100.0

class Bots
{
private:
  long size;
  Vector * bots;
  long botsIteratorCount;  // iterator variable for patrolmen iteratoring
  static double const visionRadius;

public:
  Bots () {}
  Bots (long n)
    :botsIteratorCount(0)
    ,size(n)
    {
    bots = new Vector[n];

    double div = 2*PI / size;
    for (long i = 0;i < size; ++i) {
      double r =  getRandomNumberBetween(BOT_INIT_RADIUS/4.0, BOT_INIT_RADIUS);
      double t =  getRandomNumberBetween(0.0, 2*PI);
      double d =  getRandomNumberBetween(0.0, 2*PI);
      bots[i](r*std::cos(div * i), r*std::sin(div * i), d);
    }
  }


  long getSize () const { return size; }
  double getVisionRaduis () const { return visionRadius; }

  Vector nextBot () { return bots[botsIteratorCount++]; }

  long indexOfBotIterator () {
    if (botsIteratorCount<size) return botsIteratorCount;
    else {
      botsIteratorCount = 0;
      return -1;
      }
  }

  void forward () {
    for ( long here = 0;here < size; ++here ) {
      bots[here].forward();
      //then turn
      for ( long another = 0; another < size; ++another ) {
        if ( another != here && bots[here].distanceTo(bots[another]) <= visionRadius) {
          // when meet another bot...
          // turn...

          double thetaTotal = 0;
          double thetaCount = 0;

          for (long other = 0; other < size; ++ other) {
            if( (other != here)
              &&(bots[here].distanceTo(bots[other]) <= visionRadius/2.0)
              ) {
              double dx = bots[other].getX() - bots[here].getX();
              double dy = bots[other].getY() - bots[here].getY();
              double dtheta = std::atan2(dy, dx);
              thetaTotal += dtheta;
              thetaCount += 1;
            }
          }
          if (thetaCount > 0) {
            bots[here].setTheta(thetaTotal/thetaCount > PI ? thetaTotal/thetaCount - PI : thetaTotal/thetaCount + PI);
          }
        }
      }
    }
  }
};

double const Bots::visionRadius = BOT_VISION_RADUIS;

#undef PI
#undef BOT_VISION_RADUIS
#undef BOT_INIT_RADIUS

// Bots constructor binding
EMSCRIPTEN_BINDINGS(bots) {
  class_<Bots>("Bots")
    .constructor<long>()
    .property("size", &Bots::getSize)
    .function("nextBot", &Bots::nextBot)
    .function("indexOfBotIterator", &Bots::indexOfBotIterator)
    .function("forward", &Bots::forward)
    .property("visionRaduis", &Bots::getVisionRaduis);
    ;
}

#endif
