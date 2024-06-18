#pragma once

#include <memory>
#include <functional>

namespace sw
{
    /// @todo remove later
    struct PlaneCoordinnates 
    {
        uint32_t _x {0};
        uint32_t _y {0};
    };

/// @brief interface for gemeboards
class IMap
{
public:
    IMap() = default;
    virtual ~IMap() = default;

    /// @brief To chec that the map can be used in a game
    virtual bool isValid() const = 0;

    /// @brief Spawn any unit
    virtual bool spawn(const std::uint32_t unitId, const std::uint32_t x, const std::uint32_t y) = 0; /// @todo fix dependency on coordinates

    /// @brief Move any unit
    virtual bool move(const std::uint32_t unitId, const std::uint32_t targetX, const std::uint32_t targetY) = 0; /// @todo fix dependency on coordinates

    virtual PlaneCoordinnates getCoordinnates(const std::uint32_t unitId, bool &ok) const = 0;

    /// @brief Write unitId to vector if there are unitis in the radius
    virtual void csanRadius(const uint32_t unitId, const uint32_t r, std::vector<uint32_t> &units) const = 0;

protected:
    /// @brief To check that point is avaliable to move or spawn
    virtual bool isVacant(const std::uint32_t x, const std::uint32_t y) const = 0; /// @todo fix dependency on coordinates
};

} // namespace sw