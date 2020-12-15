#pragma once
#include "ArcInfo.h"
#include "VertexNode.h"
#include <concepts>
template<std::equality_comparable, std::copy_constructible>
struct VertexNode;
template<std::equality_comparable T, std::copy_constructible U>
struct ArcNode {
    VertexNode<T, U> *adjVexPosition;
    U arcInfo;
    ArcNode *next;
};