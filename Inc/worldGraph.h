#ifndef WORLD_GRAPH_H_DEFINED
#define WORLD_GRAPH_H_DEFINED
#include <common.h>
#include <bits/stdc++.h>

typedef enum
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    NO_PATH,
} direction;

using namespace std;

class worldGraph
{
private:
    GRAPH_TYPE *_graph;
    int _target;
    int _position;

public:
    worldGraph(GRAPH_TYPE *graph);
    ~worldGraph() {}
    void setTarget(int targetID);
    void setPosition(int PositionID);
    direction getDirection();
};

#endif