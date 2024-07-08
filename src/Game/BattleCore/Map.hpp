#pragma once

#include "Point.h"

#include <memory>
#include <cstdint>
#include <vector>
#include <unordered_map>


namespace sw
{

class IUnit;

class Map
{
    std::vector<std::vector<uint32_t>>  _model; ///< first vector is for x indices, second is for y indices; second contains unit ids (default 0)
    std::unordered_map<uint32_t, Point> _points;

public:
    
    Map() = default;

    void init(const uint32_t width, const uint32_t height);

    /// @brief To check that the map can be used in a game
    bool isValid() const;

    /// @brief Spawn any unit
    bool spawn(std::shared_ptr<IUnit> unit, const std::uint32_t x, const std::uint32_t y);

    /// @brief Move any unit
    bool move(const std::uint32_t unitId, const std::uint32_t targetX, const std::uint32_t targetY);
    
    /// @brief To check that point is avaliable to move or spawn
    bool isVacant(const std::uint32_t x, const std::uint32_t y) const;

    /// @brief Find units around unit with unitId 
    void scanAround(const uint32_t unitId, const uint32_t radiusBegin, const uint32_t radiusEnd, std::vector<uint32_t> &unitIds) const;

    /// @brief Remove any unit
    bool kill(const std::uint32_t unitId);

    Point getUnitPoint(const std::uint32_t unitId, bool &ok) const;
};

} // namespace sw