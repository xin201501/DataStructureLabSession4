#pragma once
#include "ArcInfo.h"
#include "ArcNode.h"
#include "VertexNode.h"
#include "adjacencylistgraph/ArcNodeIterator/ArcNodeIterator.h"
#include "adjacencylistgraph/ArcNodeIterator/ConstArcNodeIterator.h"
#include <algorithm>
#include <concepts>
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>
template<std::equality_comparable>
class ArcNodeIterator;
template<std::equality_comparable T>
class AdjacencyListGraph {
    friend class ArcNodeIterator<T>;
    friend class ConstArcNodeIterator<T>;

public:
    using VertexNodeType = VertexNode<T>;
    using VertexNodePtr = VertexNodeType *;
    using ArcNodeType = ArcNode<T>;
    using ArcNodeTypePtr = ArcNodeType *;

private:
    std::list<VertexNodeType> vertices;
    std::unordered_map<T, VertexNodePtr> vertexPosition;
    size_t vexNum;
    size_t arcNum;
    VertexNodePtr locateVertex(const T &x) {
        auto mapPosition = vertexPosition.find(x);
        if (mapPosition == vertexPosition.cend()) {
            return nullptr;
        }
        return mapPosition->second;
    }
    auto locateVertex(const T &x) const {
        return const_cast<AdjacencyListGraph *>(this)->locateVertex();
    }
    bool containsVertex(const T &x) const {
        return locateVertex() == nullptr;
    }

public:
    explicit AdjacencyListGraph(const std::list<VertexNodeType> &vertices, size_t vexNum = 0, size_t arcNum = 0)
        : vertices(vertices), vexNum(vexNum), arcNum(arcNum) {}
    explicit AdjacencyListGraph(std::list<VertexNodeType> &&vertices = {}, size_t vexNum = 0, size_t arcNum = 0) : vertices(std::move(vertices)), vexNum(vexNum), arcNum(arcNum) {}
    AdjacencyListGraph(const AdjacencyListGraph &another) = default;
    AdjacencyListGraph(AdjacencyListGraph &&another) noexcept : vertices(std::move(another.vertices)), vexNum(another.vexNum), arcNum(another.vexNum) {}
    ConstArcNodeIterator<T> getConstAdjacencyNodesIteratorOfAVertex(const T &x) {
        auto xPosition = locateVertex(x);
        return xPosition ? ConstArcNodeIterator(xPosition->first) : ConstArcNodeIterator<T>(nullptr);
    }
    ConstArcNodeIterator<T> getConstAdjacencyNodesIteratorOfAVertex(const VertexNode<T> &x) {
        return {x.first};
    }
    ConstArcNodeIterator<T> getConstAdjacencyNodesEndIterator() {
        return {};
    }
    ArcNodeIterator<T> getAdjacencyNodesIteratorOfAVertex(const T &x) {
        auto xPosition = locateVertex(x);
        return xPosition ? ArcNodeIterator(xPosition->first) : ArcNodeIterator<T>(nullptr);
    }
    ArcNodeIterator<T> getAdjacencyNodesIteratorOfAVertex(VertexNode<T> &x) {
        return {x.first};
    }
    ArcNodeIterator<T> getAdjacencyNodesEndIterator() {
        return {};
    }
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
        vertices.push_front(newVertex);
        vertexPosition.insert({newVertex, &*vertices.front()});
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
        }
        vertices.erase(vertexPosition);
        vertexPosition.erase(vertex);
        return true;
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
        ArcNodeTypePtr xNodePointer = vertexPosition[x];
        ArcNodeTypePtr newArcNode = new ArcNode{.adjVexPosition = xNodePointer, .next = xNodePointer->next};
        xNodePointer->next = newArcNode;
    }
    bool removeEdge(const T &x, const T &y) {
        auto xPosition = locateVertex(x), yPosition = locateVertex(y);
        if (!xPosition || !yPosition) {
            return false;
        }
        for (auto begin = getAdjacencyNodesIteratorOfAVertex(*xPosition), end = getAdjacencyNodesEndIterator(), pre = end;
             begin != end; pre = begin, begin++) {
            if (begin->adjVexPosition == yPosition) {
                auto tmp = &*begin;
                pre->next = begin->next;
                delete tmp;
                return true;
            }
        }
        return false;
    }
};