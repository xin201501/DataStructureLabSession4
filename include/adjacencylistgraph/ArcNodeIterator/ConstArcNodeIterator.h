#pragma once
#include "../AdjacencyListGraph.h"
#include "../ArcNode.h"
#include <concepts>
#include <iterator>
#include <utility>
template<std::equality_comparable, std::copy_constructible>
class AdjacencyListGraph;
template<std::equality_comparable T, std::copy_constructible U>
class ConstArcNodeIterator : public std::iterator<std::forward_iterator_tag, ArcNode<T, U>> {
    friend class AdjacencyListGraph<T, U>;

protected:
    ArcNode<T, U> *current;
    void ensureNotOperatingAnEndIterator() {
        if (current == nullptr) {
            throw std::logic_error("Cannot dereference an end pointer!");
        }
    }

public:
    explicit ConstArcNodeIterator() : current(nullptr) {}
    explicit ConstArcNodeIterator(VertexNode<T, U> *vertex) : current(vertex->first) {}
    explicit ConstArcNodeIterator(VertexNode<T, U> &vertex) : current(vertex.first) {}
    explicit ConstArcNodeIterator(ArcNode<T, U> *current) : current(current) {}
    explicit ConstArcNodeIterator(ArcNode<T, U> &node) : current(node.next) {}
    bool operator==(const ConstArcNodeIterator &another) const {
        return current == another.current;
    }
    bool operator!=(const ConstArcNodeIterator &another) const {
        return !(*this == another);
    }
    const ArcNode<T, U> &operator*() {
        ensureNotOperatingAnEndIterator();
        return *current;
    }
    const ArcNode<T, U> *operator->() {
        return current;
    }
    const ArcNode<T, U> *data() {
        return current;
    }
    ConstArcNodeIterator &operator++() {
        ensureNotOperatingAnEndIterator();
        current = current->next;
        return *this;
    }
    ConstArcNodeIterator operator++(int) {
        ensureNotOperatingAnEndIterator();
        ConstArcNodeIterator it = *this;
        ++*this;
        return it;
    }
    ConstArcNodeIterator next() {
        ensureNotOperatingAnEndIterator();
        return {current->next};
    }
};