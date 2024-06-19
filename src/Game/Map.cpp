#include "Map.hpp"


#include "UnitRegister.hpp"

namespace sw
{

Map::Map(const std::shared_ptr<const UnitRegister> &units, const uint32_t width, const uint32_t height)
    : _units{units}
    , _model(height, std::vector<uint32_t>(width, 0))
{
}

bool Map::isValid() const
{
    return _model.size() && _model[0].size();
}

bool Map::spawn(const std::uint32_t unitId, const std::uint32_t x, const std::uint32_t y)
{
    if (!isVacant(x, y))
    {
        return false;
    }
    _model[x][y] = unitId;
    _coords.emplace(unitId, PlaneCoordinnates{x, y});
    return true;
}

bool Map::move(const std::uint32_t unitId, const std::uint32_t targetX, const std::uint32_t targetY)
{
    if (!isVacant(targetX, targetY))
    {
        return false;/// @todo impossible
    }
    auto it = _coords.find(unitId);
    if (it == _coords.end())
    {
        return false;
    }

    PlaneCoordinnates &curr = it->second;
    _model[curr._x][curr._y] = 0;
    curr._x = targetX;
    curr._y = targetY;
    _model[targetX][targetY] = unitId;
    return true;
}

bool Map::isVacant(const std::uint32_t x, const std::uint32_t y) const
{
    if (x > _model.size() - 1 || (_model.size() && y > _model[0].size() - 1))
    {
        return false;
    }
    return _model[x][y] == 0;
}

PlaneCoordinnates Map::getCoordinnates(const std::uint32_t unitId, bool &ok) const
{
    auto it = _coords.find(unitId);
    if (it == _coords.end())
    {
        ok = false;
        return PlaneCoordinnates();
    }
    ok = true;
    return it->second;
}

void Map::scanAround(const uint32_t unitId, const uint32_t r, std::vector<std::shared_ptr<IUnit>> &units) const
{
    bool ok = false;
    auto unitPoint = getCoordinnates(unitId, ok);
    if (!ok)
    {
        return;
    }

    for (int x = unitPoint._x - r; x <= unitPoint._x + r; ++x)
    {   if (x < 0 || x > _model.size() - 1)
        {
            continue;
        }
        for (int y = unitPoint._y - r ; y <= unitPoint._y + r; ++y)
        {
            if (y < 0 || y > _model[0].size() - 1)
            {
                continue;
            }
            if (!isVacant(x, y) && (x != unitPoint._x || y != unitPoint._y))
            {
                auto unit = _units->find(_model[x][y]); 
                if (unit)
                {
                    units.push_back(unit);
                }
            }
        }
    }

}

bool Map::kill(const std::uint32_t unitId)
{
    auto it = _coords.find(unitId);
    if (it == _coords.end())
    {
        return false;
    }
    _model[it->second._x][it->second._y] = 0;
    _coords.erase(it);
    return true;
}

std::shared_ptr<IUnit> Map::getUnit(const std::uint32_t unitId) const
{
    return _units->find(unitId);
}

} // namespace sw