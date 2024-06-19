#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace sw
{

class IUnit;
class UnitRegister;

struct Point 
{
    uint32_t x {0};
    uint32_t y {0};
};

/// @brief Rectangle gameboard
class Map
{
    std::vector<std::vector<uint32_t>> _model; ///< first vector is for x indices, second is for y indices; second contains unit ids (default 0)
    std::unordered_map<uint32_t, Point> _coords;
    std::shared_ptr<const UnitRegister> _units;

public:
    Map(const std::shared_ptr<const UnitRegister> &units, const uint32_t width, const uint32_t height);

    /// @brief To check that the map can be used in a game
    bool isValid() const;

    /// @brief Spawn any unit
    bool spawn(const std::uint32_t unitId, const std::uint32_t x, const std::uint32_t y);

    /// @brief Move any unit
    bool move(const std::uint32_t unitId, const std::uint32_t targetX, const std::uint32_t targetY);

    /// @brief To check that point is avaliable to move or spawn
    bool isVacant(const std::uint32_t x, const std::uint32_t y) const;

    /// @brief Find units around unit with unitId in the radius
    void scanAround(const uint32_t unitId, const uint32_t r, std::vector<std::shared_ptr<IUnit>> &units) const;

    /// @brief Remove any unit
    bool kill(const std::uint32_t unitId);

    Point getPoint(const std::uint32_t unitId, bool &ok) const;
    std::shared_ptr<IUnit> getUnit(const std::uint32_t unitId) const;
};

} // namespace sw