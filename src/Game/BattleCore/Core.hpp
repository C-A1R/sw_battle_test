#pragma once

#include "Point.h"
#include "Map.hpp"
#include "UnitRegister.hpp"

#include <vector>
#include <memory>

namespace sw
{

class IUnit;
class IUnitAction;

class Core : public std::enable_shared_from_this<Core>
{
    std::unique_ptr<Map>            _map;
    std::unique_ptr<UnitRegister>   _units;

public:
    Core();

    void initMap(const uint32_t width, const uint32_t height);
    bool isValid() const;
    bool tick(const uint32_t tick);

    void addUnitAction(const uint32_t unitId, const std::shared_ptr<IUnitAction> &act);

    Point getUnitPoint(const std::uint32_t unitId, bool &ok) const;
    std::shared_ptr<IUnit> getUnit(const std::uint32_t id) const;

    bool spawn(const std::shared_ptr<IUnit> &unit, const std::uint32_t x, const std::uint32_t y);
    bool move(const std::uint32_t unitId, const std::uint32_t targetX, const std::uint32_t targetY);
    void scanAround(const uint32_t unitId, const uint32_t radiusBegin, const uint32_t range, std::vector<std::shared_ptr<IUnit>> &units) const;
};

} // namespace sw
