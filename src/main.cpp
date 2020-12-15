#include "MapArcInfo.h"
#include "adjacencylistgraph/AdjacencyListGraph.h"
int main() {
    AdjacencyListGraph<int> graph;
    graph.getAdjacencyNodesEndIterator();
    graph.getConstAdjacencyNodesEndIterator();
    graph.removeEdge(1, 3);
}