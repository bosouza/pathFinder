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

/* worldGraph represents the world as a graph: crossings are vertices and lines are edges.
By specifying the starting position and orientation, as well as the target position, the world
graph is able to provide navigation by calling the getDirection() method. After each call to
getDirection(), the caller is expected to specify whether the last instruction succeeded or not
using the instructionCompleted() or instructionFailed() methods.
*/
class worldGraph
{
private:
    // _target holds the target initially passed in the constructor method
    ii _target;
    // position holds the current position the robot is in, and is initiated in the constructor with the starting position
    ii _position;
    // _graph holds the data structure that represents the whole graph (vertices and edges)
    GRAPH_TYPE _graph;
    // _orientation holds the current orientation of the robot, and is initiated in the constructor with the starting orientation
    orientationTypedef _orientation;
    // _instructedDirection holds the last direction returned by the getDirection method
    orientationTypedef _instructedDirection;

public:
    // position and target are the starting and target positions; initialOrientation is the
    // starting orientation of the robot; graphSize is the size of the graph. Bellow an example
    // of size 4 world graph:
    // O O O O
    //   | |
    // O-O-O-O
    //   | |
    // O-O-O-O
    //   | |
    // O O O O
    worldGraph(int graphSize, ii position, ii target, orientationTypedef initialOrientation);
    ~worldGraph() {}
    void print();
    // getDirection returns an instruction in the form of a direction to turn and then
    // follow the line until reaching the next vertex or finding an obstacle in the middle.
    direction getDirection();
    // instructionCompleted informs the world graph that the last instruction given by
    // getDirection could be completed successfully (i.e. following the line specified
    // by getDirection led to a vertex). It's assumed that once the robot reached its
    // destination it maintained the orientation of the guide line just traveled.
    void instructionCompleted();
    // instructionFailed informs the world graph that the last instruction given by
    // getDirection couldn't be completed (i.e there was an obstacle in the way). It's
    // assumed that once encountering an obstacle, the robot turns 180 degrees and
    // returns to the last visited vertex, maintaining its orientation upon arrival.
    void instructionFailed();
};

#endif