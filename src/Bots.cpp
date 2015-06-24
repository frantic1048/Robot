#ifndef __ROBOT_BOTS_INCLUDED__
#define __ROBOT_BOTS_INCLUDED__

#include <cstdlib>
#include <cmath>

#include <emscripten/bind.h>
using namespace emscripten;

class Bot;
double getRandomNumberBetween(double lowerBound, double upperBound);

#define PI 3.141592653589793
#define EPS_APR 0.3
#define EPS_ZERO 0.0000001
#define BOT_VISION_RADUIS 30
#define BOT_INIT_RADIUS 100
#define BOT_MOVE_MODE_FREE 1
#define BOT_MOVE_MODE_EDGE 2

class Bots
{
private:
  long size;
  Bot * bots;
  int * botMoveMode; // move mode of bot
  long botsIteratorCount;  // iterator variable for patrolmen iteratoring
  long mapSize;
  Point * mapVerticies;
  static double const visionRadius;

public:
  Bots () {}
  Bots (long n,Point * _mapVerticies,long _mapSize)
    :botsIteratorCount(0)
    ,size(n)
    ,mapSize(_mapSize)
    {
    mapVerticies = _mapVerticies;
    bots = new Bot[n];
    botMoveMode = new int[n];

    double div = 2*PI / size;
    for (long i = 0;i < size; ++i) {
      double r =  getRandomNumberBetween(BOT_INIT_RADIUS/4.0, BOT_INIT_RADIUS);
      double t =  getRandomNumberBetween(0.0, 2*PI);
      double d =  getRandomNumberBetween(0.0, 2*PI);
      bots[i](r*std::cos(div * i), r*std::sin(div * i), d);
      botMoveMode[i] = BOT_MOVE_MODE_FREE;
    }
  }


  long getSize () const { return size; }
  double getVisionRaduis () const { return visionRadius; }

  Bot nextBot () { return bots[botsIteratorCount++]; }

  long indexOfBotIterator () {
    if (botsIteratorCount<size) return botsIteratorCount;
    else {
      botsIteratorCount = 0;
      return -1;
      }
  }

  bool isOnEdge (Bot bot, Point vertexP, Point vertexN) {
    double distP, // distance to previous vertex
           distN, // distance to next vertex
           distPN;// distance between distP, distN

    distP = bot.distanceTo(vertexP);
    distN = bot.distanceTo(vertexN);
    distPN = vertexP.distanceTo(vertexN);
    return (std::abs(distP + distN - distPN) < EPS_APR) ? true : false;
  }

  void freeTurn (long here) {
    Bot &thisBot = bots[here];
    for ( long another = 0; another < size; ++another ) {
      if ( another != here && thisBot.distanceTo(bots[another]) <= visionRadius) {
        // another bot in vision...
        // compute new direction...

        double neighborCount = 0,
               xTotal = 0,
               yTotal = 0,
               thetaAveReverse,
               dx,
               dy,
               dist,
               modularLength,
               weight;

        for (long other = 0; other < size; ++ other) {
          if( (other != here)
            &&((dist = thisBot.distanceTo(bots[other])) <= visionRadius)
            ) {
            dx = bots[other].getX() - thisBot.getX();
            dy = bots[other].getY() - thisBot.getY();
            modularLength = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
            weight = std::pow(((int) visionRadius/dist), 2);

            xTotal += dx/modularLength * weight;
            yTotal += dy/modularLength * weight;
            neighborCount += weight;
          }
        }

        thetaAveReverse = std::atan2(-yTotal/neighborCount,-xTotal/neighborCount);
        thisBot.setTheta(thetaAveReverse);
      }
      // bots[another] not in vision
      // do nothing
    }
  }

  void forward () {
    for ( long here = 0;here < size; ++here ) {
      if (botMoveMode[here] == BOT_MOVE_MODE_FREE) {
        // first, detect if bot reaches edge
        if (isOnEdge(bots[here],
                     mapVerticies[mapSize - 1],
                     mapVerticies[0])) {
           botMoveMode[here] = BOT_MOVE_MODE_EDGE;
           bots[here].setTheta(0);
         }
        for (long vert = 1;vert < mapSize;++vert){
          if (botMoveMode[here] == BOT_MOVE_MODE_EDGE) {break;}
          if (isOnEdge(bots[here],
                       mapVerticies[vert-1],
                       mapVerticies[vert])) {

              // reach edge,change to Edge mode
               botMoveMode[here] = BOT_MOVE_MODE_EDGE;
               bots[here].setTheta(0);
             }
        }

        if (botMoveMode[here] == BOT_MOVE_MODE_FREE) {
          // not on edge,then freemode turn
          freeTurn(here);
        }

      } else if (botMoveMode[here] == BOT_MOVE_MODE_EDGE) {
        // EDGE MODE
      }

      // direction computed, normally forward 1 step
      bots[here].forward();
    }
  }
};

double const Bots::visionRadius = BOT_VISION_RADUIS;

#undef PI
#undef EPS_APR
#undef EPS_ZERO
#undef BOT_VISION_RADUIS
#undef BOT_INIT_RADIUS
#undef BOT_MOVE_MODE_FREE
#undef BOT_MOVE_MODE_EDGE

// Bots constructor binding
EMSCRIPTEN_BINDINGS(bots) {
  class_<Bots>("Bots")
    .constructor<long, Point *, long>()
    .property("size", &Bots::getSize)
    .function("nextBot", &Bots::nextBot)
    .function("indexOfBotIterator", &Bots::indexOfBotIterator)
    .function("forward", &Bots::forward)
    .property("visionRaduis", &Bots::getVisionRaduis);
    ;
}

#endif
