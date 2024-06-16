#include "RectangleMap.hpp"

#include <stdexcept>

namespace sw
{

RectangleMap::RectangleMap(const uint32_t width, const uint32_t height)
    : _model(height, std::vector<uint32_t>(width, 0))
{
}

bool RectangleMap::isValid() const
{
    return _model.size() && _model[0].size();
}

bool RectangleMap::spawn(const std::uint32_t unitId, const std::uint32_t x, const std::uint32_t y)
{
    if (!isVacant(x, y))
    {
        return false;
    }
    _model[x][y] = unitId;
    return true;
}

bool RectangleMap::isVacant(const std::uint32_t x, const std::uint32_t y) const
{
    if (x > _model.size() - 1 || (_model.size() && y > _model[0].size() - 1))
    {
        throw std::invalid_argument(std::string("Error: map doesn't contain the point [") + std::to_string(x) + ';' + std::to_string(y) +']');
    }
    return _model[x][y] == 0;
}

} // namespace sw