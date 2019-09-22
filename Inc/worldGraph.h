#ifndef WORLD_GRAPH_H_DEFINED
#define WORLD_GRAPH_H_DEFINED
#include <bits/stdc++.h>
#define ii std::pair<int, int>
#define GRAPH_TYPE std::map<ii, std::map<orientationTypedef, ii>>

typedef enum
{
    FORWARD = 0,
    LEFT = 1,
    BACKWARD = 2,
    RIGHT = 3,
    NO_PATH,
} direction;

typedef enum
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
} orientationTypedef;

class worldGraph
{
private:
    ii _target;
    ii _position;
    GRAPH_TYPE _graph;
    orientationTypedef _orientation;
    orientationTypedef _instructedDirection;

public:
    worldGraph(int graphSize, ii position, ii target, orientationTypedef initialOrientation);
    ~worldGraph() {}
    void print();
    //getDirection returns an instruction in the form of a direction to turn and then
    //follow the line until reaching the next vertex or finding an obstacle in the middle.
    direction getDirection();
    //instructionCompleted informs the world graph that the last instruction given by
    //getDirection could be completed successfully (i.e. following the line specified
    //by getDirection led to a vertex). It's assumed that once the robot reached its
    //destination it maintained the orientation of the guide line just traveled.
    void instructionCompleted();
    //instructionFailed informs the world graph that the last instruction given by
    //getDirection couldn't be completed (i.e there was an obstacle in the way). It's
    //assumed that once encountering an obstacle, the robot turns 180 degrees and
    //returns to the last visited vertex, maintaining its orientation upon arrival.
    void instructionFailed();
};

#endif