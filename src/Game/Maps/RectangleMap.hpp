#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "IMap.hpp"

namespace sw
{


/// @brief Rectangle gameboard
class RectangleMap : public IMap
{
    std::vector<std::vector<uint32_t>> _model; ///< first vector is for x indices, second is for y indices; second contains unit ids (default 0)
    std::unordered_map<uint32_t, PlaneCoordinnates> _coords;

public:
    RectangleMap(const uint32_t width, const uint32_t height);

protected:
    bool isValid() const override;
    bool spawn(const std::uint32_t unitId, const std::uint32_t x, const std::uint32_t y) override;
    bool move(const std::uint32_t unitId, const std::uint32_t targetX, const std::uint32_t targetY) override;

private:
    bool isVacant(const std::uint32_t x, const std::uint32_t y) const override;
    PlaneCoordinnates getCoordinnates(const std::uint32_t unitId, bool &ok) const override;
    void csanRadius(const uint32_t unitId, const uint32_t r, std::vector<uint32_t> &units) const override;
};

} // namespace sw