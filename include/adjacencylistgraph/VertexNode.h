#pragma once
#include "ArcNode.h"
template<typename>
struct ArcNode;
template<typename T>
struct VertexNode {
    T info;
    ArcNode<T> *first = nullptr;
};