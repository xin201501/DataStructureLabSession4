#include "MapArcInfo.h"
#include "adjacencylistgraph/AdjacencyListGraph.h"
int main() {
    AdjacencyListGraph<int> graph;
    graph.firstNeighborIterator(1);
    graph.afterLastNeighborIterator();
    graph.removeEdge(1, 3);
}