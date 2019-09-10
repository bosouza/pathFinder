#include <worldGraph.h>

worldGraph::worldGraph(GRAPH_TYPE *graph)
{
    _graph = graph;
}

void worldGraph::setTarget(int target)
{
    _target = target;
}

void worldGraph::setPosition(int position)
{
    _position = position;
}

direction worldGraph::getDirection()
{
    return NO_PATH;
}