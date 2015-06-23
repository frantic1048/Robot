#ifndef __ROBOTO_MAP_INCLUDED__
#define __ROBOTO_MAP_INCLUDED__

#include <cstdlib>

#include <emscripten/bind.h>
using namespace emscripten;

#define MAP_INIT_RADIUS 500

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

    // TODO: randomize init
  }

  long getSize () const {
    return size;
  }

  Point nextVertex () { return vertices[verticesIteratorCount++]; }

  long indexOfVertexIterator () {
    if (verticesIteratorCount<size)
      return verticesIteratorCount;
    else {
      verticesIteratorCount = 0;
      return -1;
    }
  }

};

#undef MAP_INIT_RADIUS

// Map constructor binding
EMSCRIPTEN_BINDINGS(map) {
  class_<Map>("Map")
    .constructor<long>()
    .property("size", &Map::getSize)
    .function("nextVertex", &Map::nextVertex)
    .function("indexOfVertexIterator", &Map::indexOfVertexIterator);
}

#endif
