#pragma once

#include <cstdint>
#include <cmath>

namespace sw
{

struct Point 
{
    uint32_t x {0};
    uint32_t y {0};

    Point() = default;
    Point(const uint32_t x, const uint32_t y)
    {
        this->x = x;
        this->y = y;
    }
    Point(const Point &p)
    {
        x = p.x;
        y = p.y;
    }

    static double distance(const Point &l, const Point &r)
    {
        return std::sqrt(std::pow(std::max(l.x, r.x) - std::min(l.x, r.x), 2) 
                       + std::pow(std::max(l.y, r.y) - std::min(l.y, r.y), 2));
    }
    bool operator==(const Point &p)
    {
        return x == p.x && y == p.y;
    }
    bool operator!=(const Point &p)
    {
        return !(p == *this);
    }
}; 

} // namespace sw