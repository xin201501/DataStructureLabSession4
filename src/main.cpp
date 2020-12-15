#include "MapArcInfo.h"
#include "adjacencylistgraphfactory/MapAdjacencyListGraphFactory.h"
#include <iostream>
int main() {
    system("chcp 65001");
    MapAdjacencyListGraphFactory factory;
    auto cityMap = factory.getBean();
    std::cout << "test ended" << std::endl;
}