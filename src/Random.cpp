#ifndef __ROBOT_RANDOM_INCLUDED__
#define __ROBOT_RANDOM_INCLUDED__

#include <random>

#include <emscripten/bind.h>
using namespace emscripten;

// get a random number between lowerbound and upperbound
double getRandomNumberBetween(double lowerBound, double upperBound) {
  std::random_device rd;
  std::default_random_engine re(rd());
  std::uniform_real_distribution<double> dist(lowerBound, upperBound);
  return dist(re);
}

EMSCRIPTEN_BINDINGS(random) {
  function("getRandomNumberBetween",&getRandomNumberBetween);
}
#endif
