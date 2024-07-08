#include "Archer.hpp"

#include "../BattleCore/Core.hpp"
#include "../../IO/System/EventLog.hpp"
#include "../../IO/Events/UnitAttacked.hpp"

#include <algorithm>

namespace sw
{

Archer::Archer(const uint32_t id, const uint32_t hp, const uint32_t strength, const uint32_t range, const uint32_t agility)
    : Warrior(id, hp, strength)
    , _range{range}
    , _agility{agility}
{
}


bool Archer::findAndAtack(const std::shared_ptr<Core> &core, const int32_t tick) const
{
    std::vector<std::shared_ptr<IUnit>> enemies;
    core->scanAround(getId(), _minShotDistance, _range, enemies);
    if (enemies.empty())
    {
        return Warrior::findAndAtack(core, tick);
    }
    return rangeAtack(core, enemies, tick);
}

bool Archer::rangeAtack(const std::shared_ptr<Core> &core, const std::vector<std::shared_ptr<IUnit>> &enemies, const int32_t tick) const
{
    if (enemies.empty() || !core)
    {
        return false;
    }

    std::shared_ptr<IUnit> target = chooseTarget(core, enemies);
    if (!target)
    {
        return false;
    }
    const uint32_t harm = target->damage(_agility);
    EventLog::instance().log(tick, io::UnitAttacked{getId(), target->getId(), harm, target->getHp()});
    return true;
}

std::shared_ptr<IUnit> Archer::chooseTarget(const std::shared_ptr<Core> &core, const std::vector<std::shared_ptr<IUnit>> &enemies) const
{
    if (enemies.empty() || !core)
    {
        return nullptr;
    }

    if (enemies.size() == 1)
    {
        return enemies[0];
    }

    uint32_t distance = 0;
    bool ok = false;
    auto myPoint = core->getUnitPoint(getId(), ok);
    if (!ok)
    {
        return nullptr;
    }

    double minDist = std::numeric_limits<double>::max();
    std::unordered_map<uint32_t, double> distancesToThis;
    for (const auto &e : enemies)
    {
        bool ok = false;
        const Point &ePoint = core->getUnitPoint(e->getId(), ok);
        if (!ok)
        {
            continue;
        }
        
        const double dist = Point::distance(ePoint, myPoint);
        distancesToThis.emplace(e->getId(), dist);
        if (dist < minDist)
        {
            minDist = dist;
        }
    }
    if (distancesToThis.empty())
    {
        return nullptr;
    }

    std::vector<std::shared_ptr<IUnit>> nearestUnits;
    std::copy_if(enemies.cbegin(), enemies.cend(), std::back_inserter(nearestUnits), [minDist, &distancesToThis](const std::shared_ptr<IUnit> &unit)
    {
        return distancesToThis.at(unit->getId()) == minDist;
    });

    if (nearestUnits.size() == 1)
    {
        return nearestUnits[0];
    }

    return Warrior::chooseTarget(nearestUnits);
}

} // namespace sw