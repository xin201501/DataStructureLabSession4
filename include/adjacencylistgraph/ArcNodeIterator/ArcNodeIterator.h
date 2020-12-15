#pragma once
#include "../AdjacencyListGraph.h"
#include "../ArcNode.h"
#include "ConstArcNodeIterator.h"
#include <concepts>
#include <iterator>
#include <utility>
template<std::equality_comparable, std::copy_constructible>
class ConstArcNodeIterator;
template<std::equality_comparable, std::copy_constructible>
class AdjacencyListGraph;
template<std::equality_comparable T, std::copy_constructible U>
class ArcNodeIterator : public ConstArcNodeIterator<T, U> {
    friend class AdjacencyListGraph<T, U>;

public:
    using ConstArcNodeIterator<T, U>::ConstArcNodeIterator;
    ArcNodeIterator(const ArcNodeIterator &) = default;
    ArcNode<T, U> &operator*() {
        this->ensureNotOperatingAnEndIterator();
        return *this->current;
    }
    ArcNode<T, U> *operator->() {
        return this->current;
    }
    ArcNode<T, U> *data() {
        return this->current;
    }
    ArcNodeIterator next() {
        this->ensureNotOperatingAnEndIterator();
        return {this->current->next};
    }
};