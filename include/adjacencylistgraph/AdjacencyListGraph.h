#pragma once
#include "ArcInfo.h"
#include "ArcNode.h"
#include "VertexNode.h"
#include "adjacencylistgraph/ArcNodeIterator/ArcNodeIterator.h"
#include "adjacencylistgraph/ArcNodeIterator/ConstArcNodeIterator.h"
#include <algorithm>
#include <concepts>
#include <iostream>
#include <list>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
template<std::equality_comparable, std::copy_constructible>
class ArcNodeIterator;
template<std::equality_comparable T, std::copy_constructible U>
class AdjacencyListGraph {
    friend class ArcNodeIterator<T, U>;
    friend class ConstArcNodeIterator<T, U>;

public:
    using VertexNodeType = VertexNode<T, U>;
    using VertexNodePtr = VertexNodeType *;
    using ArcNodeType = ArcNode<T, U>;
    using ArcNodeTypePtr = ArcNodeType *;

private:
    std::list<VertexNodeType> vertices;
    std::unordered_map<T, VertexNodePtr> vertexPosition;
    size_t vexNum;
    size_t arcNum;
    VertexNodePtr locateVertex(const T &x) {
        return vertexPosition.contains(x) ? vertexPosition[x] : nullptr;
    }
    const VertexNodePtr locateVertex(const T &x) const {
        return const_cast<AdjacencyListGraph *>(this)->locateVertex(x);
    }
    [[nodiscard]] bool containsVertex(const T &x) const {
        return vertexPosition.contains(x);
    }
    std::string DFS(const VertexNode<T, U> &node, std::unordered_map<std::string_view, bool> &visited) {
        if (visited[node.info]) {
            return "";
        }
        std::string result = "city name:" + node.info + '\n';
        visited[node.info] = true;
        for (auto it = constFirstNeighborIterator(&node), end = constAfterLastNeighborIterator(); it != end; it++) {
            if (!visited[it->adjVexPosition->info]) {
                result += DFS(*it->adjVexPosition, visited);
            }
        }
        return result;
    }
    //TODO 无法确定弧节点存储的信息类的确切类型,函数操作与该信息相关只能不定义该函数,目前无法在存储信息基类上使用成员函数模板，
    // 把该函数定义为虚函数也不可行，所有使用模版的人需自己定义该函数
    [[nodiscard]] std::pair<size_t, std::string> Dijkstra(const T &start, const T &end) {
    }

public:
    [[maybe_unused]] explicit AdjacencyListGraph(const std::list<VertexNodeType> &vertices, size_t vexNum = 0, size_t arcNum = 0)
        : vertices(vertices), vexNum(vexNum), arcNum(arcNum) {}
    [[maybe_unused]] explicit AdjacencyListGraph(std::list<VertexNodeType> &&vertices = {}, size_t vexNum = 0, size_t arcNum = 0) : vertices(std::move(vertices)), vexNum(vexNum), arcNum(arcNum) {}
    AdjacencyListGraph(const AdjacencyListGraph &another) = default;
    AdjacencyListGraph(AdjacencyListGraph &&another) noexcept : vertices(std::move(another.vertices)), vertexPosition(std::move(another.vertexPosition)), vexNum(another.vexNum), arcNum(another.vexNum) {}
    ~AdjacencyListGraph() {
        for (auto &vertex : vertices) {
            ArcNode<T, U> *arcNodePtr = vertex.first, *tmp;
            while (arcNodePtr) {
                tmp = arcNodePtr->next;
                delete arcNodePtr;
                arcNodePtr = tmp;
            }
        }
    }
    [[nodiscard]] auto constFirstNeighborIterator(const T &x) {
        auto xPosition = locateVertex(x);
        return xPosition ? ConstArcNodeIterator<T, U>(xPosition) : ConstArcNodeIterator<T, U>{};
    }
    [[nodiscard]] auto constFirstNeighborIterator(const VertexNode<T, U> &x) {
        return ConstArcNodeIterator<T, U>{x.first};
    }
    [[nodiscard]] auto constFirstNeighborIterator(const VertexNode<T, U> *x) {
        return ConstArcNodeIterator<T, U>{x->first};
    }
    [[nodiscard]] auto constAfterLastNeighborIterator() {
        return ConstArcNodeIterator<T, U>{};
    }
    [[nodiscard]] auto firstNeighborIterator(const T &x) {
        auto xPosition = locateVertex(x);
        return xPosition ? ArcNodeIterator<T, U>(xPosition) : ArcNodeIterator<T, U>{};
    }
    [[nodiscard]] auto firstNeighborIterator(VertexNode<T, U> &x) {
        return ArcNodeIterator<T, U>{x.first};
    }
    [[nodiscard]] auto firstNeighborIterator(VertexNode<T, U> *x) {
        return ArcNodeIterator<T, U>{x->first};
    }
    [[nodiscard]] auto afterLastNeighborIterator() {
        return ArcNodeIterator<T, U>{};
    }
    [[nodiscard]] bool isAdjacent(const T &x, const T &y) const {
        auto xPosition = locateVertex(x);
        if (!xPosition) {
            return false;
        }
        auto firstAdjacencyVertex = xPosition->first;
        while (firstAdjacencyVertex) {
            if (firstAdjacencyVertex->adjVexPosition->info == y) {
                return true;
            }
            firstAdjacencyVertex = firstAdjacencyVertex->next;
        }
        return false;
    }
    [[nodiscard]] bool isAdjacent(ConstArcNodeIterator<T, U> &x, ConstArcNodeIterator<T, U> &y) {
        return isAdjacent(*x, *y);
    }
    std::vector<const ArcNodeTypePtr> neighbors(const T &x) const {
        auto xPointer = locateVertex(x);
        if (!xPointer) {
            return {};
        }
        std::vector<const ArcNodeTypePtr> result;
        ArcNodeTypePtr firstAdjacencyVertex = xPointer->first;
        while (firstAdjacencyVertex) {
            result.push_back(firstAdjacencyVertex);
            firstAdjacencyVertex = firstAdjacencyVertex->next;
        }
        return result;
    }
    bool insertVertexIfAbsent(const T &newVertex) {
        if (containsVertex(newVertex)) {
            return false;
        }
        vertices.push_front(VertexNode<T, U>{newVertex, nullptr});
        vertexPosition[newVertex] = &vertices.front();
        vexNum++;
        return true;
    }
    bool deleteVertex(const T &vertex) {
        auto vertexPointer = locateVertex(vertex);
        if (!vertexPointer) {
            return false;
        }
        ArcNodeTypePtr firstAdjacencyVertex = vertexPointer->first, tmp;
        while (firstAdjacencyVertex) {
            tmp = firstAdjacencyVertex->next;
            delete firstAdjacencyVertex;
            firstAdjacencyVertex = tmp;
            arcNum--;
        }
        vertices.erase(*vertexPointer);
        vertexPosition.erase(vertex);
        vexNum--;
        return true;
    }

    void addEdge(const T &x, const T &y, const U &arcInfo) {
        insertVertexIfAbsent(x);
        insertVertexIfAbsent(y);
        if (isAdjacent(x, y)) {
            return;
        }
        VertexNodePtr xNodePointer = vertexPosition[x], yNodePointer = vertexPosition[y];
        ArcNodeTypePtr newArcNode = new ArcNode<T, U>{yNodePointer, arcInfo, xNodePointer->first};
        xNodePointer->first = newArcNode;
        arcNum++;
    }
    bool removeEdge(const T &x, const T &y) {
        auto xLocation = locateVertex(x), yLocation = locateVertex(y);
        if (!xLocation || !yLocation) {
            return false;
        }
        for (auto it = firstNeighborIterator(xLocation), tmp = it, end = afterLastNeighborIterator(); it != end; it++) {
            if (it->adjVexPosition == yLocation) {
                tmp->next = it->next;
                delete it.data();
                arcNum--;
                return true;
            }
        }
        return false;
    }
    std::optional<U> getEdgeValue(const T &x, const T &y) {
        auto xLocation = locateVertex(x), yLocation = locateVertex(y);
        if (!xLocation || !yLocation) {
            return std::nullopt;
        }
        for (auto it = constFirstNeighborIterator(xLocation), end = constAfterLastNeighborIterator(); it != end; it++) {
            if (it->adjVexPosition->info == y) {
                return {it->arcInfo};
            }
        }
        return std::nullopt;
    }
    template<typename... Args>
    void setEdgeValue(const T &x, const T &y, Args... newInfos) {
        auto xLocation = locateVertex(x), yLocation = locateVertex(y);
        if (!x || !y) {
            return;
        }
        for (auto it = firstNeighborIterator(xLocation), end = afterLastNeighborIterator(); it != end; it++) {
            if (it->adjVexPosition == yLocation) {
                it->arcInfo = U{std::forward<Args>(newInfos)...};
            }
        }
    }
    void setEdgeValue(const T &x, const T &y, const ArcInfo &newInfo) {
        if (x == y) {
            return;
        }
        auto xLocation = locateVertex(x), yLocation = locateVertex(y);
        if (!x || !y) {
            return;
        }
        for (auto it = firstNeighborIterator(xLocation), end = afterLastNeighborIterator(); it != end; it++) {
            if (it->adjVexPosition == yLocation) {
                ArcInfo *oldInfo = it->arcInfo;
                it->arcInfo = newInfo;
                delete oldInfo;
            }
        }
    }
    std::string BFS() {
        std::unordered_map<std::string_view, bool> visited;
        auto eachArcNodeEndIteratorOfAVertex = constAfterLastNeighborIterator();
        std::string result;
        for (const auto &vertex : vertices) {
            if (visited[vertex.info]) {
                continue;
            }
            result = result + "city name:" + vertex.info + '\n';
            for (auto it = constFirstNeighborIterator(&vertex); it != eachArcNodeEndIteratorOfAVertex; it++) {
                std::string_view neighborName = it->adjVexPosition->info;
                if (!visited[neighborName]) {
                    result = result + "city name:" + neighborName.data() + '\n';
                    visited[neighborName] = true;
                }
            }
        }
        return result;
    }
    std::string DFS() {
        std::unordered_map<std::string_view, bool> visited;
        std::string result;
        for (const auto &vertex : vertices) {
            result += DFS(vertex, visited);
        }
        return result;
    }
    //TODO 因不知道具体操作无法写通用函数
    [[nodiscard]] std::string getMinCostPath(std::string_view start, std::string_view end) {
    }
};
//TODO 专用于本实验的单源最短路径算法
template<>
[[nodiscard]] std::pair<size_t, std::string> AdjacencyListGraph<std::string, MapArcInfo>::Dijkstra(const std::string &start, const std::string &end) {
    std::unordered_map<std::string, size_t> currentMinDistance;
    //    std::unordered_map<std::string, std::string> currentMinPath;
    std::unordered_map<std::string, bool> hasDetermined;
    std::unordered_map<std::string, std::string> path;
    for (const auto &vertex : vertices) {
        auto edgeInfo = getEdgeValue(start, vertex.info);
        currentMinDistance[vertex.info] = isAdjacent(start, vertex.info) && edgeInfo ? edgeInfo->getPrice() : std::numeric_limits<size_t>::max();
        hasDetermined[vertex.info] = false;
        path[vertex.info] = start + "->" + vertex.info;
    }
    currentMinDistance[start] = 0;
    hasDetermined[start] = true;
    size_t count = 1;
    while (count < vexNum) {
        size_t min = std::numeric_limits<size_t>::max();
        std::string tmp;
        for (const auto &vertex : vertices) {
            if (!hasDetermined[vertex.info] && currentMinDistance[vertex.info] < min) {
                min = currentMinDistance[vertex.info];
                tmp = vertex.info;
            }
        }
        if (tmp == end) {
            return {currentMinDistance[end], path[end]};
        }
        hasDetermined[tmp] = true;
        count++;
        for (const auto &vertex : vertices) {
            auto edgeInfo = getEdgeValue(tmp.data(), vertex.info);
            if (size_t currentSinglePathPrice = edgeInfo ? edgeInfo->getPrice() : std::numeric_limits<size_t>::max(), possibleMinPrice = currentMinDistance[tmp.data()] + currentSinglePathPrice;
                !hasDetermined[vertex.info] && currentSinglePathPrice != std::numeric_limits<size_t>::max() && possibleMinPrice < currentMinDistance[vertex.info]) {
                currentMinDistance[vertex.info] = possibleMinPrice;
                path[vertex.info] = path[tmp.data()] + "->" + vertex.info;
            }
        }
    }
    return {std::numeric_limits<size_t>::max(), end};
}
//生成最短路径信息的函数
template<>
[[nodiscard]] std::string AdjacencyListGraph<std::string, MapArcInfo>::getMinCostPath(std::string_view start, std::string_view end) {
    auto [price, path] = Dijkstra(start.data(), end.data());
    std::string result = "path which has the minimum cost from city ";
    result = result + start.data() + " to " + end.data() + " is:";
    if (price == std::numeric_limits<size_t>::max()) {
        result += "No path between the two cities!\n";
    } else {
        result = result + "path:" + path + " cost:" + std::to_string(price);
    }
    return result;
}