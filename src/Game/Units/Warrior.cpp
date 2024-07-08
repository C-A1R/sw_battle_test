#include "Warrior.hpp"

#include "../BattleCore/Core.hpp"
#include "../../IO/System/EventLog.hpp"
#include "../../IO/Events/UnitAttacked.hpp"

#include <algorithm>

namespace sw
{

Warrior::Warrior(const uint32_t id, const uint32_t hp, const uint32_t strength)
    : Unit(id, hp)
    , _strength{strength}
{
}

bool Warrior::findAndAtack(const std::shared_ptr<Core> &core, const int32_t tick) const
{
    std::vector<std::shared_ptr<IUnit>> enemies;
    core->scanAround(getId(), _meleeAtackDistance, _meleeAtackDistance, enemies);
    if (enemies.empty())
    {
        return false;
    }
    return meleeAtack(enemies, tick);
}

bool Warrior::meleeAtack(const std::vector<std::shared_ptr<IUnit>> &enemies, const int32_t tick) const
{
    if (enemies.empty())
    {
        return false;
    }

    std::shared_ptr<IUnit> target = chooseTarget(enemies);
    if (!target)
    {
        return false;
    }
    const uint32_t harm = target->damage(_strength);
    EventLog::instance().log(tick, io::UnitAttacked{getId(), target->getId(), harm, target->getHp()});
    return true;
}

std::shared_ptr<IUnit> Warrior::chooseTarget(const std::vector<std::shared_ptr<IUnit>> &enemies) const
{
    if (enemies.empty())
    {
        return nullptr;
    }

    if (enemies.size() == 1)
    {
        return enemies[0];
    }
    
    auto minHpIter = std::min_element(enemies.cbegin(), enemies.cend(), [](const std::shared_ptr<IUnit> &l, const std::shared_ptr<IUnit> &r)
    {
        return l->getHp() < r->getHp();
    });
    std::vector<std::shared_ptr<IUnit>> min_hp_units;
    std::copy_if(enemies.cbegin(), enemies.cend(), std::back_inserter(min_hp_units), [&minHpIter](const std::shared_ptr<IUnit> &unit)
    {
        return unit->getHp() == (*minHpIter)->getHp();
    });
    if (min_hp_units.empty())
    {
        return nullptr;
    }

    if (min_hp_units.size() == 1)
    {
        return min_hp_units[0];
    }

    std::sort(min_hp_units.begin(), min_hp_units.end(), [](const std::shared_ptr<IUnit> &l, const std::shared_ptr<IUnit> &r)
    {
        return l->getId() < r->getId();
    });
    return min_hp_units[0];
}

} // namespace sw
