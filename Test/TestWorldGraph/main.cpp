#include <worldGraph.h>
#include <bits/stdc++.h>

using namespace std;
int main()
{
    worldGraph graph(4, {1, 1}, {1, 2}, SOUTH);
    graph.print();
    direction dir = graph.getDirection();
    cout << "getDirection: " << dir << endl;
    graph.instructionFailed();
    dir = graph.getDirection();
    cout << "getDirection: " << dir << endl;
    return 0;
}