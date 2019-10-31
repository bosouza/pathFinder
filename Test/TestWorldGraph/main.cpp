#include <worldGraph.h>
#include <bits/stdc++.h>

using namespace std;
int main()
{
    worldGraph graph(6, {2, 0}, {3, 5}, NORTH);
    // graph.print();
    // direction dir = graph.getDirection();
    // cout << "getDirection: " << dir << endl;
    // graph.instructionFailed();
    // dir = graph.getDirection();
    // cout << "getDirection: " << dir << endl;
    for (int i = 0; i < 6; i++)
    {
        direction dir = graph.getDirection();
        graph.instructionCompleted();
        cout << "getDirection: " << dir << endl;
    }
    return 0;
}