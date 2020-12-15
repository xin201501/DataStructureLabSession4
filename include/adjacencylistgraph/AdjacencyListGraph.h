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
        return mapPosition == vertexPosition.cend() ? nullptr : mapPosition->second;
    }
    const VertexNodePtr locateVertex(const T &x) const {
        return const_cast<AdjacencyListGraph *>(this)->locateVertex();
    }
    bool containsVertex(const T &x) const {
        return locateVertex() == nullptr;
    }

public:
    [[maybe_unused]] explicit AdjacencyListGraph(const std::list<VertexNodeType> &vertices, size_t vexNum = 0, size_t arcNum = 0)
        : vertices(vertices), vexNum(vexNum), arcNum(arcNum) {}
    [[maybe_unused]] explicit AdjacencyListGraph(std::list<VertexNodeType> &&vertices = {}, size_t vexNum = 0, size_t arcNum = 0) : vertices(std::move(vertices)), vexNum(vexNum), arcNum(arcNum) {}
    AdjacencyListGraph(const AdjacencyListGraph &another) = default;
    AdjacencyListGraph(AdjacencyListGraph &&another) noexcept : vertices(std::move(another.vertices)), vexNum(another.vexNum), arcNum(another.vexNum) {}
    auto constFirstNeighborIterator(const T &x) {
        auto xPosition = locateVertex(x);
        return xPosition ? ConstArcNodeIterator<T>(xPosition) : ConstArcNodeIterator<T>(nullptr);
    }
    auto constFirstNeighborIterator(const VertexNode<T> &x) {
        return ConstArcNodeIterator<T>{x.first};
    }
    auto constFirstNeighborIterator(const VertexNode<T> *x) {
        return ConstArcNodeIterator<T>{x->first};
    }
    auto constAfterLastNeighborIterator() {
        return ConstArcNodeIterator<T>{};
    }
    auto firstNeighborIterator(const T &x) {
        auto xPosition = locateVertex(x);
        return xPosition ? ArcNodeIterator<T>(xPosition) : ArcNodeIterator<T>();
    }
    auto firstNeighborIterator(VertexNode<T> &x) {
        return ArcNodeIterator<T>{x.first};
    }
    auto firstNeighborIterator(VertexNode<T> *x) {
        return ArcNodeIterator<T>{x->first};
    }
    auto afterLastNeighborIterator() {
        return ArcNodeIterator<T>{};
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
            arcNum--;
        }
        vertices.erase(vertexPosition);
        vertexPosition.erase(vertex);
        return true;
    }

    void addEdge(const T &x, const T &y, ArcInfo *arcInfo = nullptr) {
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
        ArcNodeTypePtr newArcNode = new ArcNode{xNodePointer, arcInfo, xNodePointer->next};
        xNodePointer->next = newArcNode;
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
    ArcInfo *getEdgeValue(const T &x, const T &y) {
        auto xLocation = locateVertex(x), yLocation = locateVertex(y);
        if (!x || !y) {
            return nullptr;
        }
        for (auto it = constFirstNeighborIterator(xLocation), end = constAfterLastNeighborIterator(); it != end; it++) {
            if (it->adjVexPosition == yLocation) {
                return it->arcInfo;
            }
        }
        return nullptr;
    }
    template<typename... Args>
    void setEdgeValue(const T &x, const T &y, Args... newInfos) {
        auto xLocation = locateVertex(x), yLocation = locateVertex(y);
        if (!x || !y) {
            return;
        }
        for (auto it = firstNeighborIterator(xLocation), end = afterLastNeighborIterator(); it != end; it++) {
            if (it->adjVexPosition == yLocation) {
                ArcInfo *oldInfo = it->arcInfo;
                it->arcInfo = new auto {std::forward<Args>(newInfos)...};
                delete oldInfo;
            }
        }
    }
    void setEdgeValue(const T &x, const T &y, ArcInfo *newInfo) {
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
};