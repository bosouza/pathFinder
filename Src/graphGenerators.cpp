#include <bits/stdc++.h>
#include <common.h>
#include <graphGenerators.h>

void generateDefaultGraph(int size, GRAPH_TYPE &graph)
{
    if (size < 2)
        throw "cannot create default graph with size < 2";
    graph.clear();
    vertexIDMapper mapper(size);
    for (int x = 1; x < size - 1; x++)
    {
        for (int y = 1; y < size - 1; y++)
        {
            graph[mapper.getVertexID(x, y)].insert(mapper.getVertexID(x - 1, y - 1));
            graph[mapper.getVertexID(x, y)].insert(mapper.getVertexID(x - 1, y + 1));
            graph[mapper.getVertexID(x, y)].insert(mapper.getVertexID(x + 1, y - 1));
            graph[mapper.getVertexID(x, y)].insert(mapper.getVertexID(x + 1, y + 1));
        }
    }
}