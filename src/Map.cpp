#ifndef __ROBOT_MAP_INCLUDED__
#define __ROBOT_MAP_INCLUDED__

#include <cstdlib>
#include <cmath>

#include <emscripten/bind.h>
using namespace emscripten;

#define MAP_INIT_RADIUS 300
#define PI 3.141592653589793

class Point;

class Map{
private:
  long size;
  Point * vertices;
  long verticesIteratorCount;

public:
  Map () {}
  Map (long _size)
  :size(_size)
  {
    vertices = new Point[size];

    double div = 2*PI/size,
           x = 0.0,
           y = 0.0,
           theta = 0.0,
           r = 0.0;

    for (long here = 0;here < size;++here) {
      // more vertex,less randomization range
      r = MAP_INIT_RADIUS + getRandomNumberBetween(-(800.0/size<160?800/size:160),0);
      x = r*std::sin(theta);
      y = r*std::cos(theta);
      vertices[here](x,y);
      theta = theta + div;
    }
  }

  long getSize () const {
    return size;
  }

  Point * getVertices() const {
    return vertices;
  }

  Point nextVertex () { return vertices[verticesIteratorCount++]; }

  long indexOfVertexIterator () {
    if (verticesIteratorCount < size)
      return verticesIteratorCount;
    else {
      verticesIteratorCount = 0;
      return -1;
    }
  }

};

#undef MAP_INIT_RADIUS
#undef PI

// Map constructor binding
EMSCRIPTEN_BINDINGS(map) {
  class_<Map>("Map")
    .constructor<long>()
    .property("size", &Map::getSize)
    .function("nextVertex", &Map::nextVertex)
    .function("indexOfVertexIterator", &Map::indexOfVertexIterator);
}

#endif
