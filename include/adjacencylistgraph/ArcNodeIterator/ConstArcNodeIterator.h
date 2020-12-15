#pragma once
#include "../AdjacencyListGraph.h"
#include "../ArcNode.h"
#include <concepts>
#include <iterator>
#include <utility>
template<std::equality_comparable>
class AdjacencyListGraph;
template<std::equality_comparable T>
class ConstArcNodeIterator : public std::iterator<std::forward_iterator_tag, ArcNode<T>> {
    friend class AdjacencyListGraph<T>;

protected:
    ArcNode<T> *current;
    void ensureNotOperatingAnEndIterator() {
        if (current == nullptr) {
            throw std::logic_error("Cannot dereference an end pointer!");
        }
    }

public:
    ConstArcNodeIterator(ArcNode<T> *current = nullptr) : current(current) {}
    ConstArcNodeIterator(ArcNode<T> &node) : current(node.next) {}
    bool operator==(const ConstArcNodeIterator &another) const {
        return current == another.current;
    }
    bool operator!=(const ConstArcNodeIterator &another) const {
        return !(*this == another);
    }
    const ArcNode<T> &operator*() {
        ensureNotOperatingAnEndIterator();
        return *current;
    }
    const ArcNode<T> *operator->() {
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
};