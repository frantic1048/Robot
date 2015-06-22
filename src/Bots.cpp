#ifndef __ROBOTO_BOTS_INCLUDED__
#define __ROBOTO_BOTS_INCLUDED__

#include <cstdlib>
#include <ctime>

#include <emscripten/bind.h>
using namespace emscripten;

#define BOT_INIT_RADIUS 200

class Vector;

// Patrolman viewprot distance
#define RADUIS 20
#define PI 3.141592653589793

class Bots
{
private:
  long size;
  Vector * bots;
  long botsIteratorCount;  // iterator variable for patrolmen iteratoring

public:
  Bots () {}
  Bots (long n)
    :botsIteratorCount(0)
    ,size(n)
    {
    bots = new Vector[n];
    // TODO: randomize init
  }

  long getSize () const {
    return size;
  }

  Vector nextBot () { return bots[botsIteratorCount++]; }

  long isEndBot () {
    if (botsIteratorCount<size) return botsIteratorCount;
    else {
      botsIteratorCount = 0;
      return -1;
      }
  }

  void forward () {
    for ( long i = 0;i < size; ++i ) {
      bots[i].forward();
      //then turn
      for ( long j = 0; j < size; ++j ) {
        if ( j != i && bots[i].distanceTo(bots[j]) <= RADUIS) {
          // when meet another bot
          // turn 1 degree clockwisely
          bots[i].setTheta(bots[i].getTheta() + PI * 1.0 /180);
        }
      }
    }
  }
};

#undef RADIUS
#undef PI

// Bots constructor binding
EMSCRIPTEN_BINDINGS(bots) {
  class_<Bots>("Bots")
    .constructor<long>()
    .property("size", &Bots::getSize)
    .function("nextBot", &Bots::nextBot)
    .function("isEndBot", &Bots::isEndBot)
    .function("forward", &Bots::forward)
    ;
}

#endif
