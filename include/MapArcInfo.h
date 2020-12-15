#pragma once
#include "adjacencylistgraph/ArcInfo.h"
#include <cstddef>
class MapArcInfo : public ArcInfo {
private:
    size_t distance;
    size_t price;

public:
    explicit MapArcInfo(size_t distance, size_t price) : distance(distance), price(price) {}
    [[nodiscard]] size_t getDistance() const {
        return distance;
    }
    void setDistance(size_t distance_) {
        distance = distance_;
    }
    [[nodiscard]] size_t getPrice() const {
        return price;
    }
    void setPrice(size_t price_) {
        price = price_;
    }
};