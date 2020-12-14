#pragma once
#include "ArcInfo.h"
#include "VertexNode.h"
template<typename>
struct VertexNode;
template<typename T>
struct ArcNode {
    VertexNode<T> *adjVexPosition;
    ArcInfo arcInfo;
    ArcNode *next;
};