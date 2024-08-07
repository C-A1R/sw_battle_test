#include "Map.hpp"

#include "UnitRegister.hpp"
#include "../Units/IUnit.hpp"

namespace sw
{

void Map::init(const uint32_t width, const uint32_t height)
{
    _model = std::vector(height, std::vector<uint32_t>(width, 0));
}

bool Map::isValid() const
{
    return _model.size() && _model[0].size();
}

bool Map::spawn(std::shared_ptr<IUnit> unit, const std::uint32_t x, const std::uint32_t y)
{
    if (!isVacant(x, y))
    {
        return false;
    }
    _model[x][y] = unit->getId();
    _points.emplace(unit->getId(), Point{x, y});
    return true;
}

bool Map::move(const std::uint32_t unitId, const std::uint32_t targetX, const std::uint32_t targetY)
{
    if (!isVacant(targetX, targetY))
    {
        return false;/// @todo impossible
    }
    auto it = _points.find(unitId);
    if (it == _points.end())
    {
        return false;
    }

    Point &curr = it->second;
    _model[curr.x][curr.y] = 0;
    curr.x = targetX;
    curr.y = targetY;
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

Point Map::getUnitPoint(const std::uint32_t unitId, bool &ok) const
{
    auto it = _points.find(unitId);
    if (it == _points.end())
    {
        ok = false;
        return Point();
    }
    ok = true;
    return it->second;
}

void Map::scanAround(const uint32_t unitId, const uint32_t radiusBegin, const uint32_t range, std::vector<uint32_t> &unitIds) const
{
    bool ok = false;
    auto unitPoint = getUnitPoint(unitId, ok);
    if (!ok)
    {
        return;
    }

    const int ux = unitPoint.x;
    const int uy = unitPoint.y;
    const int _x = ux + range;
    const int _y = uy + range;
    for (int x = ux - range; x <= _x; ++x)
    {   
        if (x < 0 || x > _model.size() - 1)
            continue;

        for (int y = uy - range; y <= _y; ++y)
        {
            if (y < 0 || y > _model[0].size() - 1)
                continue;

            if (x > ux - radiusBegin && x < ux + radiusBegin
                && y > uy - radiusBegin && y < uy + radiusBegin)
            {
                continue;
            }

            if (!isVacant(x, y) && (x != ux || y != uy))
            {
                unitIds.emplace_back(_model[x][y]);

                ///@todo
                // auto unit = _units->find(_model[x][y]); 
                // if (unit && unit->getHp() > 0)
                // {
                //     units.push_back(unit);
                // }
            }
        }
    }
}

bool Map::kill(const std::uint32_t unitId)
{
    auto it = _points.find(unitId);
    if (it == _points.end())
    {
        return false;
    }
    _model[it->second.x][it->second.y] = 0;
    _points.erase(it);
    return true;
}

} // namespace sw