#include <bits/stdc++.h>

#include <main.h>
#include <worldGraph.h>

using namespace std;
const pair<int, int> target(5, 3);
const pair<int, int> initial(1, 5);
const orientationTypedef initialOrientation = SOUTH;
const float allignDistance = 0.2;

int main(void)
{
  worldGraph graph(6, initial, target, initialOrientation);

  InitAVG();

  direction path;
  while ((path = graph.getDirection()) != NO_PATH)
  {
    switch (path)
    {
    case LEFT:
      TurnLeftFor(800);
      break;
    case RIGHT:
      TurnRightFor(800);
      break;
    case BACKWARD:
      TurnRightFor(1400);
      break;
    }
    if (FollowLine(allignDistance) == AGV_OK)
      graph.instructionCompleted();
    else
    {
      TurnRightFor(1400);
      if (FollowLine(allignDistance) == AGV_ERROR)
        SignalError();
      graph.instructionFailed();
    }
  }
}
