#pragma once
#include "ArcInfo.h"
#include "VertexNode.h"
#include <concepts>
template<typename>
struct VertexNode;
template<typename T>
struct ArcNode {
    VertexNode<T> *adjVexPosition;
    ArcInfo *arcInfo;
    ArcNode *next;
};