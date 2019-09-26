#include <bits/stdc++.h>

#include <main.h>
#include <worldGraph.h>

using namespace std;
const pair<int, int> target(0, 1);
const pair<int, int> initial(3, 4);
const orientationTypedef initialOrientation = EAST;
const float allignDistance = 0.1;

int main(void)
{
  worldGraph graph(5, initial, target, initialOrientation);

  TurnLeft(1000);
  while (1)
    ;

  InitAVG();
  TurnAngle(20);
  FollowLine(0.1);

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
      TurnAngle(turn);
    if (FollowLine(allignDistance) == AGV_OK)
      graph.instructionCompleted();
    else
    {
      TurnAngle(3.1415);
      if (FollowLine(allignDistance) == AGV_ERROR)
        SignalError();
      graph.instructionFailed();
    }
  }
}
