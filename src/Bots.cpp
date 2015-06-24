#ifndef __ROBOT_BOTS_INCLUDED__
#define __ROBOT_BOTS_INCLUDED__

#include <cstdlib>
#include <cmath>

#include <emscripten/bind.h>
using namespace emscripten;

class Bot;
double getRandomNumberBetween(double lowerBound, double upperBound);

#define PI 3.141592653589793
#define EPS_EDGE 1.5
#define EPS_VERTEX 2
#define DIRECTION_UNDEFINED 1000
#define DIRECTION_INFINITE 99999
#define BOT_VISION_RADUIS 25
#define BOT_INIT_RADIUS 50
#define BOT_MOVE_MODE_FREE 1
#define BOT_MOVE_MODE_EDGE 2
#define BOT_EDGE_UNDEFINED -1
#define BOT_EDGE_CLOCKWISE 1
#define BOT_EDGE_ANTI_CLOCKWISE 2

class Bots
{
private:
  long size;
  Bot * bots;
  int * botMoveMode; // move mode of bot
  int * botMoveClockwise; // move direction of bot on edge
  long botsIteratorCount;  // iterator variable for patrolmen iteratoring
  long mapSize;
  Point * mapVertices;
  static double const visionRadius;

public:
  Bots () {}
  Bots (long n,Point * _mapVertices,long _mapSize)
    :botsIteratorCount(0)
    ,size(n)
    ,mapSize(_mapSize)
    {
    mapVertices = _mapVertices;
    bots = new Bot[n];
    botMoveMode = new int[n];
    botMoveClockwise = new int[n];

    double div = 2*PI / size;
    for (long i = 0;i < size; ++i) {
      double r =  getRandomNumberBetween(BOT_INIT_RADIUS/4.0, BOT_INIT_RADIUS);
      double t =  getRandomNumberBetween(0.0, 2*PI);
      double d =  getRandomNumberBetween(0.0, 2*PI);
      bots[i](r*std::cos(div * i), r*std::sin(div * i), d);
      botMoveMode[i] = BOT_MOVE_MODE_FREE;
      botMoveClockwise[i] = BOT_EDGE_UNDEFINED;
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

  long previousVertexIndex(long here) {
    if (here > 0) {
      return here - 1;
    } else {
      return mapSize - 1;
    }
  }

  long nextVertexIndex(long here) {
    if (here < mapSize - 1) {
      return here + 1;
    } else {
      return 0;
    }
  }

  bool isOnEdge (Bot bot, Point vertexP, Point vertexN) {
    double distP, // distance to previous vertex
           distN, // distance to next vertex
           distPN;// distance between distP, distN

    distP = bot.distanceTo(vertexP);
    distN = bot.distanceTo(vertexN);
    distPN = vertexP.distanceTo(vertexN);
    if ((distP < EPS_EDGE)
      ||(distN < EPS_EDGE)
      ||(std::abs(distP + distN - distPN) < EPS_EDGE)){
      return true;
    } else {
      return false;
    }
  }

  double idealDirection (long here) {
    Bot &thisBot = bots[here];
    double neighborCount = 0,
           xTotal = 0,
           yTotal = 0,
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

    if (neighborCount > 0) {
      return std::atan2(-yTotal/neighborCount,-xTotal/neighborCount);
    } else {
      return DIRECTION_INFINITE;
    }
  }

  bool isValidDirection(double d) {
    if (d < DIRECTION_UNDEFINED){
      return true;
    } else {
      return false;
    }
  }

  void freeModeTurn (long here) {
    Bot &thisBot = bots[here];
    double i = idealDirection(here);
    if (isValidDirection(i)){
      thisBot.setTheta(i);
    }
  }

  void edgeModeTurn(long here) {
    Bot &thisBot = bots[here];
    Point A, B;// previous and next vertex of current edge
    double iDirection = idealDirection(here), // ideal direction
           rDirectionA, // first restricted direction
           rDirectionB; // second restricted direction

    // a bot on edge
    // its direction is restricted to two determinate way
    // both way is point to a vertex
    // there are tow cases

    // case 1:
    //     vertexA   vertexB
    //        |         |
    //        v         v
    //       [x] --*-- [x]
    //             ^
    //             |
    //        A<--Bot-->B
    //
    // case 2:
    //     vertexA   vertexB  vertexC
    //        |         |       |
    //        v         v       v
    //       [x] ----- [*]-----[x]
    //                  ^
    //                  |
    //             A<--Bot-->C

    // find out two available target vertex of current position
    if (isOnEdge(thisBot,mapVertices[mapSize - 1],mapVertices[0])) {
      if (thisBot.distanceTo(mapVertices[mapSize - 1]) < EPS_VERTEX) {
        // bot is on mapVertices[mapSize - 1]
        A(mapVertices[previousVertexIndex(mapSize - 1)].getX(),mapVertices[previousVertexIndex(mapSize - 1)].getY());
        B(mapVertices[nextVertexIndex(mapSize - 1)].getX(),mapVertices[nextVertexIndex(mapSize - 1)].getY());
      } else if (thisBot.distanceTo(mapVertices[0]) < EPS_VERTEX) {
        // bot is on mapVertices[0]
        A(mapVertices[previousVertexIndex(0)].getX(),mapVertices[previousVertexIndex(0)].getY());
        B(mapVertices[nextVertexIndex(0)].getX(),mapVertices[nextVertexIndex(0)].getY());
      } else {
        // bot is not on any vertex
        A(mapVertices[mapSize - 1].getX(),mapVertices[mapSize - 1].getY());
        B(mapVertices[0].getX(),mapVertices[0].getY());
      }
    } else {
      for (long vert = 1;vert < mapSize;++vert){
        if (isOnEdge(thisBot,mapVertices[vert - 1],mapVertices[vert])) {
          if (thisBot.distanceTo(mapVertices[vert - 1]) < EPS_VERTEX) {
            // bot is on mapVertices[vert - 1]
            A(mapVertices[previousVertexIndex(vert - 1)].getX(),mapVertices[previousVertexIndex(vert - 1)].getY());
            B(mapVertices[nextVertexIndex(vert - 1)].getX(),mapVertices[nextVertexIndex(vert - 1)].getY());
          } else if (thisBot.distanceTo(mapVertices[vert]) < EPS_VERTEX) {
            // bot is on mapVertices[vert]
            A(mapVertices[previousVertexIndex(vert)].getX(),mapVertices[previousVertexIndex(vert)].getY());
            B(mapVertices[nextVertexIndex(vert)].getX(),mapVertices[nextVertexIndex(vert)].getY());
          } else {
            // bot is not on any vertex
            A(mapVertices[vert - 1].getX(),mapVertices[vert - 1].getY());
            B(mapVertices[vert].getX(),mapVertices[vert].getY());
          }
          break;// exit loop
        }
      }
    }

    rDirectionA = thisBot.angleTo(A);
    rDirectionB = thisBot.angleTo(B);

    Point pd(std::cos(thisBot.getTheta()),std::sin(thisBot.getTheta())), // previous direction
          id(std::cos(iDirection),std::sin(iDirection)),                 // ideal direction
          ra(std::cos(rDirectionA),std::sin(rDirectionA)),
          rb(std::cos(rDirectionB),std::sin(rDirectionB));

    // std::cout<<"bot:("<<thisBot.getX()<<","<<thisBot.getY()<<")"<<std::endl;
    // std::cout<<"ppA:("<<A.getX()<<","<<A.getY()<<")"<<std::endl;
    // std::cout<<"ppB:("<<B.getX()<<","<<B.getY()<<")"<<std::endl;
    // std::cout<<"drA:"<<rDirectionA<<std::endl;
    // std::cout<<"drB:"<<rDirectionB<<std::endl;

    if (botMoveClockwise[here] == BOT_EDGE_UNDEFINED) {
      if (pd.distanceTo(ra) < pd.distanceTo(rb)) {
        thisBot.setTheta(rDirectionA);
        botMoveClockwise[here] = BOT_EDGE_CLOCKWISE;
      } else {
        thisBot.setTheta(rDirectionB);
        botMoveClockwise[here] = BOT_EDGE_CLOCKWISE;
      }
    } else {
      if (isValidDirection(iDirection)) {
        // pick the direction closet to idealDirection
        if (id.distanceTo(ra) < id.distanceTo(rb)) {
          thisBot.setTheta(rDirectionA);
          botMoveClockwise[here] = BOT_EDGE_CLOCKWISE;
        } else {
          thisBot.setTheta(rDirectionB);
          botMoveClockwise[here] = BOT_EDGE_ANTI_CLOCKWISE;
        }
      } else {
        // nothing to refrence
        if (botMoveClockwise[here] == BOT_EDGE_CLOCKWISE) {
          thisBot.setTheta(rDirectionA);
        } else {
          thisBot.setTheta(rDirectionB);
        }
      }
    }
  }

  void forward () {
    for ( long here = 0;here < size; ++here ) {
      if (botMoveMode[here] == BOT_MOVE_MODE_FREE) {
        // a freemode bot
        // first, detect if bot reaches edge
        if (isOnEdge(bots[here],mapVertices[mapSize - 1],mapVertices[0])) {
           botMoveMode[here] = BOT_MOVE_MODE_EDGE;// reach edge,change to Edge mode
           edgeModeTurn(here);// and turn
         } else {
           for (long vert = 1;vert < mapSize;++vert){
             if (isOnEdge(bots[here],mapVertices[vert-1],mapVertices[vert])) {
                  botMoveMode[here] = BOT_MOVE_MODE_EDGE;
                  edgeModeTurn(here);
                  break;// exit loop
                }
           }
        }

        // not on edge,then freemode turn
        if (botMoveMode[here] == BOT_MOVE_MODE_FREE) {
          freeModeTurn(here);
        }

      } else if (botMoveMode[here] == BOT_MOVE_MODE_EDGE) {
        // a edge mode bot
        // turn in edge mode
         edgeModeTurn(here);
      }

      // direction computed, normally forward 1 step
      bots[here].forward();
    }
  }
};

double const Bots::visionRadius = BOT_VISION_RADUIS;

#undef PI
#undef EPS_EDGE
#undef EPS_VERTEX
#undef BOT_VISION_RADUIS
#undef BOT_INIT_RADIUS
#undef BOT_MOVE_MODE_FREE
#undef BOT_MOVE_MODE_EDGE
#undef BOT_EDGE_UNDEFINED
#undef BOT_EDGE_CLOCKWISE
#undef BOT_EDGE_ANTI_CLOCKWISE

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
