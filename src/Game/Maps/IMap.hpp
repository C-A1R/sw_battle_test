#pragma once

#include <memory>

namespace sw
{

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

protected:
    /// @brief To check that point is avaliable to move or spawn
    virtual bool isVacant(const std::uint32_t x, const std::uint32_t y) const = 0; /// @todo fix dependency on coordinates
};

} // namespace sw