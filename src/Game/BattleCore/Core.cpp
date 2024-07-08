#include "Core.hpp"

#include "../Units/IUnit.hpp"

#include "IO/System/EventLog.hpp"
#include "IO/Events/UnitDied.hpp"

namespace sw
{

Core::Core()
    : _map{std::make_unique<Map>()}
    , _units{std::make_unique<UnitRegister>()}
{
}

void Core::initMap(const uint32_t width, const uint32_t height)
{
    if (_map->isValid())
    {
        // map has been already created
        return;
    }
    _map->init(width, height);
}

bool Core::isValid() const
{
    return _map->isValid();
}

bool Core::tick(const uint32_t tick)
{
    bool noActionsExecuted = true;

    for(auto i = _units->begin(); i != _units->end(); ++i)
    {
        auto &unit = i->second;
        if (unit->execNextAction(shared_from_this(), tick))
        {
            noActionsExecuted = false;
        }
    }

    for(auto i = _units->begin(); i != _units->end(); ++i)
    {
        auto &unit = i->second;
        if (!unit->getHp())
        {
            _map->kill(unit->getId());
            _units->remove(unit->getId());
            EventLog::instance().log(tick, io::UnitDied{unit->getId()});
        }
    }

    return !noActionsExecuted;
}

void Core::addUnitAction(const uint32_t unitId, const std::shared_ptr<IUnitAction> &act)
{
    auto unit = _units->find(unitId);
    if (!unit)
    {
        return;
    }
    unit->addAction(act);
}

Point Core::getUnitPoint(const std::uint32_t unitId, bool &ok) const
{
    return _map->getUnitPoint(unitId, ok);
}

std::shared_ptr<IUnit> Core::getUnit(const std::uint32_t id) const
{
    return _units->find(id);
}

bool Core::spawn(const std::shared_ptr<IUnit> &unit, const std::uint32_t x, const std::uint32_t y)
{
    if (!_map->spawn(unit, x, y))
    {
        return false;
    }
    _units->insert(unit);
    return true;
}

bool Core::move(const std::uint32_t unitId, const std::uint32_t targetX, const std::uint32_t targetY)
{
    return _map->move(unitId, targetX, targetY);
}

void Core::scanAround(const uint32_t unitId, const uint32_t radiusBegin, const uint32_t range, std::vector<std::shared_ptr<IUnit>> &units) const
{
    std::vector<uint32_t> unitIds;
    _map->scanAround(unitId, radiusBegin, range, unitIds);
    for (const uint32_t id : unitIds)
    {
        const auto unit = _units->find(id);
        if (unit)
        {
            units.emplace_back(std::move(unit));
        }
    }
}

} // namespace sw