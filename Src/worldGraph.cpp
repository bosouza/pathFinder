#include <worldGraph.h>

using namespace std;

int mod(int a, int b) { return (a % b + b) % b; }

/*the world is represented as a graph of size graphSize. Bellow an example of size 4 world graph:
O O O O
  | |
O-O-O-O
  | |
O-O-O-O
  | |
O O O O
the graph is represented by an adjacency list, where each vertex is represented by an pair<int,int>,
the bottom left vertex is {0,0}, and the top right {N-1,N-1} 
*/
worldGraph::worldGraph(int graphSize, ii position, ii target, orientationTypedef initialOrientation)
{
    _target = target;
    _position = position;
    _orientation = initialOrientation;
    if (graphSize < 3)
        //TODO: something to alert
        return;
    //connect the bottom most vertices to their NORTH connections and the top most ones to their
    //SOUTH connections
    for (int x = 1; x < graphSize - 1; x++)
    {
        _graph[{x, 0}][NORTH] = {x, 1};
        _graph[{x, graphSize - 1}][SOUTH] = {x, graphSize - 2};
    }

    //connect the left most vertices to their EAST connections and the right most ones to their
    //WEST connections
    for (int y = 1; y < graphSize - 1; y++)
    {
        _graph[{0, y}][EAST] = {1, y};
        _graph[{graphSize - 1, y}][WEST] = {graphSize - 2, y};
    }

    //connect all vertices not on the borders to all their 4 neighbors
    for (int x = 1; x < graphSize - 1; x++)
    {
        for (int y = 1; y < graphSize - 1; y++)
        {
            _graph[{x, y}][EAST] = {x + 1, y};
            _graph[{x, y}][SOUTH] = {x, y - 1};
            _graph[{x, y}][WEST] = {x - 1, y};
            _graph[{x, y}][NORTH] = {x, y + 1};
        }
    }
}

/* getDirection returns the direction to be followed next. To do that, it uses the current position
and orientation of the robot inside the graph. After receiving an instruction, the caller is expected
to either call instructionCompleted or instructionFailed
*/
direction worldGraph::getDirection()
{
    //the parent map associates each vertex with its parent found with the bfs. The orientation
    //is the direction we have to take from the parent to get to the child
    map<ii, pair<ii, orientationTypedef>> parent;
    //nextQueue is the bfs queue, it contains a vertex and the direction taken from its parent
    //to get to it
    queue<pair<ii, orientationTypedef>> nextQueue;
    nextQueue.push({_position, _orientation});
    while (!nextQueue.empty())
    {
        auto next = nextQueue.front();
        nextQueue.pop();

        //give preference to the next vertex in the same direction we're already going
        if (_graph[next.first].find(next.second) != _graph[next.first].end())
        {
            auto prioChild = _graph[next.first][next.second];
            if (parent.find(prioChild) == parent.end())
            {
                parent[prioChild] = {next.first, next.second};
                if (prioChild == _target)
                    goto endLoop;
                nextQueue.push({prioChild, next.second});
            }
        }
        //iterate over remaining directions
        for (auto child : _graph[next.first])
        {
            //skip if it has the same orientation as next, since that means it's the prioChild we already processed
            if (child.first == next.second)
                continue;
            if (parent.find(child.second) != parent.end())
                continue;
            parent[child.second] = {next.first, child.first};
            if (child.second == _target)
                goto endLoop;
            nextQueue.push({child.second, child.first});
        }
    }
endLoop:
    if (parent.find(_target) == parent.end())
        return NO_PATH;

    pair<ii, orientationTypedef> v = parent[_target];
    while (v.first != _position)
        v = parent[v.first];

    _instructedDirection = v.second;

    switch (mod(_orientation - v.second, 4))
    {
    case 0:
        return FORWARD;
    case 1:
        return LEFT;
    case 2:
        return BACKWARD;
    case 3:
        return RIGHT;
    }
}

//instruction completed updates the graph assuming the last instruction was successfully completed
void worldGraph::instructionCompleted()
{
    switch (_instructedDirection)
    {
    case NORTH:
        _position.second++;
        break;
    case EAST:
        _position.first++;
        break;
    case SOUTH:
        _position.second--;
        break;
    case WEST:
        _position.first--;
        break;
    }
    _orientation = _instructedDirection;
}

//instruction failed updates the graph assuming there was an obstacle found when following the las intruction
void worldGraph::instructionFailed()
{
    switch (_instructedDirection)
    {
    case NORTH:
        _orientation = SOUTH;
        _graph[{_position.first, _position.second + 1}].erase(SOUTH);
        break;
    case EAST:
        _orientation = WEST;
        _graph[{_position.first + 1, _position.second}].erase(WEST);
        break;
    case SOUTH:
        _orientation = NORTH;
        _graph[{_position.first, _position.second - 1}].erase(NORTH);
        break;
    case WEST:
        _orientation = EAST;
        _graph[{_position.first - 1, _position.second}].erase(EAST);
        break;
    }
    _graph[_position].erase(_instructedDirection);
}

void worldGraph::print()
{
    for (auto row : _graph)
    {
        cout << row.first.first << "," << row.first.second << ": " << endl;
        for (auto vertex : row.second)
        {
            cout << "   " << vertex.first << ": " << vertex.second.first << "," << vertex.second.second << endl;
        }
    }
}