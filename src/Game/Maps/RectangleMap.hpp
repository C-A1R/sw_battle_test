#pragma once

#include <cstdint>
#include <vector>

#include "IMap.hpp"

namespace sw
{

/// @brief Rectangle gameboard
class RectangleMap : public IMap
{
    std::vector<std::vector<uint32_t>> _model; ///< first vector is for x indices, second is for y indices; second contains unit ids (default 0)

public:
    RectangleMap(const uint32_t width, const uint32_t height);

protected:
    bool isValid() const override;
    bool spawn(const std::uint32_t unitId, const std::uint32_t x, const std::uint32_t y) override;

private:
    bool isVacant(const std::uint32_t x, const std::uint32_t y) const override;
};

} // namespace sw