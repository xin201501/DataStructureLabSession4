#pragma once
#include "ArcNode.h"
#include <concepts>
template<std::equality_comparable, std::copy_constructible>
struct ArcNode;
template<std::equality_comparable T, std::copy_constructible U>
struct VertexNode {
    T info;
    ArcNode<T, U> *first = nullptr;
};