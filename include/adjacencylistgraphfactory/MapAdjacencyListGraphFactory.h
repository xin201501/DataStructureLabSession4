#pragma once
#include "../MapArcInfo.h"
#include "../adjacencylistgraph/AdjacencyListGraph.h"
#include <charconv>
#include <fstream>
#include <string>
#include <string_view>
#include <tuple>
class MapAdjacencyListGraphFactory {
private:
    inline static std::string defaultFilename = "services.txt";
    std::ifstream fileReader;
    std::tuple<std::string, std::string, size_t, size_t> splitLine(std::string_view line) {
        auto firstSplitPoint = line.find_first_of(' '), secondSplitPoint = line.find_first_of(' ', firstSplitPoint),
             thirdSplitPoint = line.find_last_of(' ');
        if ((firstSplitPoint | secondSplitPoint | thirdSplitPoint) == std::string::npos) {
            throw std::invalid_argument("file format error!");
        }
        size_t distance, price;
        const char *distanceNumberStartPointer = line.data() + secondSplitPoint + 1, distanceNumberEndPointer = line.data() + thirdSplitPoint,
                   lineEndPointer = &line.back();
        if (auto [status, endPtr] = std::from_chars(distanceNumberStartPointer, distanceNumberEndPointer, distance);
            status != std::errc{} || endPtr != distanceNumberEndPointer) {
            throw std::invalid_argument("file format error!");
        }
        if (auto [status, endPtr] = std::from_chars(line.data() + thirdSplitPoint + 1, lineEndPointer, price);
            status != std::errc{} || endPtr != lineEndPointer) {
            throw std::invalid_argument("file format error!");
        }
        const char *firstCityNameEndPointer = line.data() + firstSplitPoint;
        return {std::string(line.data(), firstCityNameEndPointer), std::string(firstCityNameEndPointer + 1, distanceNumberStartPointer - 1), distance, price};
    };

public:
    explicit MapAdjacencyListGraphFactory(std::string_view filename = defaultFilename) : fileReader(filename.data()) {
        if (!fileReader.is_open()) {
            throw std::invalid_argument("file" + filename.data() + "opened failed!");
        }
    }
    AdjacencyListGraph<std::string> getBean() {
        AdjacencyListGraph<std::string> instance;
        std::string line;
        while (std::getline(fileReader, line)) {
            auto [startCity, endCity, distance, price] = splitLine(line);
            instance.addEdge(startCity, endCity, new MapArcInfo{distance, price});
        }
        return instance;
    }
    MapAdjacencyListGraphFactory(const MapAdjacencyListGraphFactory &) = delete;
};