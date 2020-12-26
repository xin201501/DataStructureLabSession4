#include "MapArcInfo.h"
#include "adjacencylistgraphfactory/MapAdjacencyListGraphFactory.h"
#include <iostream>
int main() {
    system("chcp 65001");
    MapAdjacencyListGraphFactory factory;
    auto cityMap = factory.getBean();
    std::cout << "Part 1:testing the BFS function:\n";
    std::cout << cityMap.BFS() << '\n';
    std::cout << "Part 2:testing the DFS function:\n";
    std::cout << cityMap.DFS() << '\n';
    std::cout << "Part 3: testing the min cost of every two cities:\n";
    std::string startCity, endCity;
    std::cout << "please input the names of the two cities,double 0 to end:" << std::endl;
    while (std::cin >> startCity >> endCity) {
        if (startCity == "0" && endCity == "0") {
            break;
        }
        std::cout << cityMap.getMinCostPath(startCity, endCity) << '\n';
        std::cout << "please input the names of the two cities,double 0 to end:" << std::endl;
    }
    std::cout << "test ends.\n";
}