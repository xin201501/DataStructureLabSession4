#pragma once
#include "ArcInfo.h"
#include "ArcNode.h"
#include "VertexNode.h"
#include "adjacencylistgraph/ArcNodeIterator/ArcNodeIterator.h"
#include "adjacencylistgraph/ArcNodeIterator/ConstArcNodeIterator.h"
#include <algorithm>
#include <concepts>
#include <list>
#include <queue>
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
    bool containsVertex(const T &x) const {
        return locateVertex(x) != nullptr;
    }

public:
    [[maybe_unused]] explicit AdjacencyListGraph(const std::list<VertexNodeType> &vertices, size_t vexNum = 0, size_t arcNum = 0)
        : vertices(vertices), vexNum(vexNum), arcNum(arcNum) {}
    [[maybe_unused]] explicit AdjacencyListGraph(std::list<VertexNodeType> &&vertices = {}, size_t vexNum = 0, size_t arcNum = 0) : vertices(std::move(vertices)), vexNum(vexNum), arcNum(arcNum) {}
    AdjacencyListGraph(const AdjacencyListGraph &another) = default;
    AdjacencyListGraph(AdjacencyListGraph &&another) noexcept : vertices(std::move(another.vertices)), vexNum(another.vexNum), arcNum(another.vexNum) {}
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
        ArcNodeTypePtr firstAdjacencyVertex = vertexPointer->first, *tmp;
        while (firstAdjacencyVertex) {
            tmp = firstAdjacencyVertex->next;
            delete firstAdjacencyVertex;
            firstAdjacencyVertex = tmp;
            arcNum--;
        }
        vertices.erase(vertexPosition);
        vertexPosition.erase(vertex);
        return true;
    }

    void addEdge(const T &x, const T &y, const U &arcInfo) {
        insertVertexIfAbsent(x);
        insertVertexIfAbsent(y);
        if (isAdjacent(x, y)) {
            return;
        }
        VertexNodePtr xNodePointer = vertexPosition[x];
        ArcNodeTypePtr newArcNode = new ArcNode<T, U>{xNodePointer, arcInfo, xNodePointer->first};
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
    U getEdgeValue(const T &x, const T &y) {
        auto xLocation = locateVertex(x), yLocation = locateVertex(y);
        if (!xLocation || !yLocation) {
            return U{};
        }
        for (auto it = constFirstNeighborIterator(xLocation), end = constAfterLastNeighborIterator(); it != end; it++) {
            if (it->adjVexPosition == yLocation) {
                return it->arcInfo;
            }
        }
        return U{};
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
    //TODO 无法确定弧节点存储的信息类的确切类型,函数操作与该信息相关只能不定义该函数,目前无法在存储信息基类上使用成员函数模板，
    // 把该函数定义为虚函数也不可行，所有使用模版的人需自己定义该函数
    [[nodiscard]] std::pair<size_t, std::string> Dijkstra(const T &start, const T &end) {
    }
};
//TODO 专用于本实验的单源最短路径函数
template<>
[[nodiscard]] std::pair<size_t, std::string> AdjacencyListGraph<std::string, MapArcInfo>::Dijkstra(const std::string &start, const std::string &end) {
    std::unordered_map<std::string, size_t> currentMinDistance;
    //    std::unordered_map<std::string, std::string> currentMinPath;
    std::unordered_map<std::string, bool> hasDetermined;
    std::string path = start;
    for (const auto &vertex : vertices) {
        currentMinDistance[vertex.info] = std::numeric_limits<size_t>::max();
        hasDetermined[vertex.info] = false;
    }
    currentMinDistance[start] = 0;
    //存储城市名和花费
    std::pair<std::string, size_t> info;
    //函数比较两条路上花费大小
    auto determineMin = [](const decltype(info) &a, const decltype(info) &b) { return a.second < b.second; };
    std::priority_queue<decltype(info), std::vector<decltype(info)>, decltype(determineMin)> possibleMin(determineMin);
    possibleMin.push({start, 0});
    size_t count = 1;
    while (!possibleMin.empty() && count <= vexNum) {
        auto [vertexName, price] = possibleMin.top();
        possibleMin.pop();
        hasDetermined[vertexName] = true;
        for (auto it = constFirstNeighborIterator(vertexName), endIt = constAfterLastNeighborIterator(); it != endIt; it++) {
            if (!hasDetermined[vertexName]) {
                info = {it->adjVexPosition->info, it->arcInfo.getPrice()};
                if (currentMinDistance[vertexName] > info.second) {
                    currentMinDistance[vertexName] = info.second;
                    possibleMin.push(info);
                }
            }
        }
        count++;
    }
    size_t minCost = currentMinDistance[end];
    //    for (const auto &vertex : vertices) {
    //        mapArcInfo = getEdgeValue(start, vertex.info);
    //        currentMinDistance[vertex.info] = mapArcInfo == MapArcInfo() ? mapArcInfo.getPrice() : std::numeric_limits<size_t>::max();
    //    }
}