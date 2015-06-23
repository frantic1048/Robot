#ifndef __ROBOT_SIMULATOR_INCLUDED__
#define __ROBOT_SIMULATOR_INCLUDED__

#include <cstdlib>

#include <emscripten/bind.h>
using namespace emscripten;

class Map;
class Bots;

class Simulator {
private:
  Map currentMap;
  Bots currentBots;
  long botCount;
  long mapVertexCount;
public:
  Simulator () {}
  Simulator (long _botCount, long _mapVertexCount)
    :botCount(_botCount)
    ,currentBots(_botCount)
    ,mapVertexCount(_mapVertexCount)
    ,currentMap(_mapVertexCount)
    {}

  void nextTick () {
    currentBots.forward();
  }
  Map getCurrentMap () const { return currentMap; }
  Bots getCurrentBots () const { return currentBots; }
};

// Simulator constructor binding
EMSCRIPTEN_BINDINGS(simulaort) {
  class_<Simulator>("Simulator")
    .constructor<long, long>()
    .property("currentMap", &Simulator::getCurrentMap)
    .property("currentBots", &Simulator::getCurrentBots)
    .function("nextTick", &Simulator::nextTick)
    ;
}

#endif
