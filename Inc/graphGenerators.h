#ifndef GRAPH_GENERATORS_H_DEFINED
#define GRAPH_GENERATORS_H_DEFINED
#include <common.h>
#include <bits/stdc++.h>

//generateDefaultGraph will reset the graph to a crossing roads pattern of size x size roads,
//where the border edges don't connect.Throws for size < 3. Example size 4:
//O   O   O   O
//    ^   ^
//O < O - O > O
//    |   |
//O < O - O > O
//    v   v
//O   O   O   O
void generateDefaultGraph(int size, GRAPH_TYPE &graph);

class vertexIDMapper
{
private:
    int _size;

public:
    vertexIDMapper(int size) { _size = size; }
    int getVertexID(int x, int y) { return _size * y + x; }
    int getVertexID(ii coordinate) { return getVertexID(coordinate.first, coordinate.second); }
    int getVertexY(int vertexID) { return vertexID / _size; }
    int getVertexX(int vertexID) { return vertexID % _size; }
    ii getCoordinate(int vertexID) { return ii(getVertexX(vertexID), getVertexY(vertexID)); }
};

#endif