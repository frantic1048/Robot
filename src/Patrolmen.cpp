#include <emscripten/bind.h>
#include <cstdlib>

using namespace emscripten;

// Patrolman viewprot distance
#define RADUIS 20
#define PI 3.141592653

class Patrolmen
{
public:
  Patrolmen (long n) {
    size = n;

    // NEED FIX
    patrolmen = (Vector *) std::calloc(n, sizeof(Vector));
  }

  long getSize () const {
    return size;
  }

  Vector * getPatrolmen () const {
    return patrolmen;
  }

  void forward () {
    for ( long i = 0;i < size; ++i ) {
      patrolmen[i].forward();
      //then turn
      for ( long j = 0; j < size; ++j ) {
        if ( j != i && patrolmen[i].distanceTo(patrolmen[j]) <= RADUIS) {
          // turn clockwisely 1 degree when meet another patrolman
          patrolmen[i].setTheta(patrolmen[i].getTheta() + PI * 1.0 /180);
        }
      }
    }
  }

private:
  long size;
  Vector * patrolmen;
};

// Patrolman constructor binding
EMSCRIPTEN_BINDINGS(patrolmen) {
  class_<Patrolmen>("Patrolmen")
    .constructor<long>()
    .property("size", &Patrolmen::getSize)
    .function("forward",&Patrolmen::forward)
    ;
}
