#include "Edge.h"

void Edge::addEdge(int time)
{
    this->time.push_back(time);
    this->weight++;
}
void Edge::addEdge()
{
    this->weight++;
}

Edge::Edge(void)
{
    weight=0;
}
