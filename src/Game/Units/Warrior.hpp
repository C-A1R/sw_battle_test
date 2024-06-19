#pragma once

#include <algorithm>

#include "IUnit.hpp"
#include "../Map.hpp"

namespace sw
{

class Warrior : public Unit
{
    uint32_t _strength {0};
    static const uint8_t _meleeAtackDistance {1};

public:
    Warrior(const uint32_t id, const uint32_t hp, const uint32_t strength)
        : Unit(id, hp)
        , _strength{strength}
    {
    }
    ~Warrior() override = default;

protected:
    bool findAndAtack(const std::shared_ptr<Map> &map, std::tuple<uint32_t, uint32_t, uint32_t> &t) const override
    {
        std::vector<std::shared_ptr<IUnit>> enemies;
        map->scanAround(getId(), _meleeAtackDistance, _meleeAtackDistance, enemies);
        if (enemies.empty())
        {
            return false;
        }
        return meleeAtack(enemies, t);
    }

    bool meleeAtack(const std::vector<std::shared_ptr<IUnit>> &enemies, std::tuple<uint32_t, uint32_t, uint32_t> &t) const
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
        uint32_t harm = target->damage(_strength);
        t = std::make_tuple(target->getId(), _strength, target->getHp());
        return true;
    }

    std::shared_ptr<IUnit> chooseTarget(const std::vector<std::shared_ptr<IUnit>> &enemies) const
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
};

} // namespace sw
