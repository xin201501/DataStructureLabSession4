#pragma once
#include "../AdjacencyListGraph.h"
#include "../ArcNode.h"
#include "ConstArcNodeIterator.h"
#include <concepts>
#include <iterator>
#include <utility>
template<std::equality_comparable>
class ConstArcNodeIterator;
template<std::equality_comparable>
class AdjacencyListGraph;
template<std::equality_comparable T>
class ArcNodeIterator : public ConstArcNodeIterator<T> {
    friend class AdjacencyListGraph<T>;

public:
    ArcNode<T> &operator*() {
        this->ensureNotOperatingAnEndIterator();
        return *this->current;
    }
    ArcNode<T> *operator->() {
        return this->current;
    }
};