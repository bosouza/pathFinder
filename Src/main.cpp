#include <bits/stdc++.h>

#include <main.h>
#include <common.h>
#include <graphGenerators.h>
#include <worldGraph.h>

#define SIZE 5
using namespace std;
const pair<int, int> target(0, 1);
const pair<int, int> initial(3, 4);
const float allignDistance = 0.1;
const float searchAngle = 0.5;

int main(void)
{
  vertexIDMapper mapper(SIZE);
  GRAPH_TYPE baseGraph;
  generateDefaultGraph(SIZE, baseGraph);
  worldGraph graph(&baseGraph);
  graph.setTarget(mapper.getVertexID(target));
  graph.setPosition(mapper.getVertexID(initial));

  InitAVG();
  TurnAngle(20);
  FollowLine(0.1);
  while (1)
  {
    ;
  }

  direction path;
  while ((path = graph.getDirection()) != NO_PATH)
  {
    float turn;
    switch (path)
    {
    case LEFT:
      turn = 3.1415 / 2;
      break;
    case RIGHT:
      turn = -3.1415 / 2;
      break;
    case BACKWARD:
      turn = 3.1415;
      break;
    }
    if (path != FORWARD)
      TurnAngleGrab(path, searchAngle);
  }
}
