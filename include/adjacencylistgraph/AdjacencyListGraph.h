#pragma once
#include "ArcInfo.h"
#include "ArcNode.h"
#include "VertexNode.h"
#include <algorithm>
#include <concepts>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
template<std::equality_comparable T>
class AdjacencyListGraph {
public:
    using VertexNodeType = VertexNode<T>;
    using VertexNodePtr = VertexNodeType *;
    using ArcNodeType = ArcNode<T>;
    using ArcNodeTypePtr = ArcNodeType *;

private:
    std::unordered_set<VertexNodeType> vertices;
    std::unordered_map<T, VertexNodePtr> vertexPosition;
    size_t vexNum;
    size_t arcNum;
    VertexNodePtr locateVertex(const T &x) {
        auto mapPosition = vertexPosition.find(x);
        if (!mapPosition) {
            return nullptr;
        }
        return *mapPosition;
    }
    auto locateVertex(const T &x) const {
        return const_cast<AdjacencyListGraph *>(this)->locateVertex();
    }
    bool containsVertex(const T &x) const {
        return locateVertex() == nullptr;
    }

public:
    explicit AdjacencyListGraph(const std::unordered_set<VertexNodeType> &vertices, size_t vexNum = 0, size_t arcNum = 0)
        : vertices(vertices), vexNum(vexNum), arcNum(arcNum) {}
    explicit AdjacencyListGraph(std::unordered_set<VertexNodeType> &&vertices = {}, size_t vexNum = 0, size_t arcNum = 0) : vertices(std::move(vertices)), vexNum(vexNum), arcNum(arcNum) {}
    AdjacencyListGraph(const AdjacencyListGraph &another) = default;
    AdjacencyListGraph(AdjacencyListGraph &&another) noexcept : vertices(std::move(another.vertices)), vexNum(another.vexNum), arcNum(another.vexNum) {}
    bool isAdjacent(const T &x, const T &y) const {
        auto xPosition = locateVertex(x);
        if (!xPosition) {
            return false;
        }
        auto firstAdjacencyVertex = xPosition->first;
        while (firstAdjacencyVertex) {
            if (firstAdjacencyVertex->info == y) {
                return true;
            }
            firstAdjacencyVertex = firstAdjacencyVertex->first;
        }
        return false;
    }
    std::vector<const ArcNodeTypePtr> neighbors(const T &x) const {
        auto vertexPointer = locateVertex(x);
        if (!vertexPointer) {
            return {};
        }
        std::vector<const ArcNodeTypePtr> result;
        ArcNodeTypePtr firstAdjacencyVertex = vertexPointer->first;
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
        vertices.push_back(newVertex);
        return true;
    }
    bool deleteVertex(const T &vertex) {
        auto vertexPointer = locateVertex(vertex);
        if (!vertexPointer) {
            return false;
        }
        ArcNodeTypePtr firstAdjacencyVertex = vertexPointer->next, *tmp;
        while (firstAdjacencyVertex) {
            tmp = firstAdjacencyVertex->next;
            delete firstAdjacencyVertex;
            firstAdjacencyVertex = tmp;
        }
        vertices.erase(vertexPosition);
        vertexPosition.erase(vertex);
    }

    void addEdge(const T &x, const T &y) {
        auto xPosition = locateVertex(x), yPosition = locateVertex(y);
        if (!xPosition) {
            auto [insertPosition, dummy] = vertices.insert({x, nullptr});
            vertexPosition.insert(x, &*insertPosition);
        }
        if (!yPosition) {
            vertices.insert({y, nullptr});
            auto [insertPosition, dummy] = vertices.insert({y, nullptr});
            vertexPosition.insert(y, &*insertPosition);
        }
        if (isAdjacent(x, y)) {
            return;
        }
        ArcNodeTypePtr verTexNodePointer = vertexPosition[x];
        ArcNodeTypePtr newNode = new ArcNode{.adjVexPosition = verTexNodePointer, .next = verTexNodePointer->next};
        verTexNodePointer->next = newNode;
    }
};