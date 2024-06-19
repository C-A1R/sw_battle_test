#pragma once

#include <algorithm>

#include "IUnit.hpp"
#include "../Map.hpp"

namespace sw
{

class Warrior : public Unit
{
    uint32_t _strength {0};
    static const uint8_t atackRadius = 1;

public:
    Warrior(const uint32_t id, const uint32_t hp, const uint32_t strength)
        : Unit(id, hp)
        , _strength{strength}
    {
    }
    ~Warrior() override = default;

protected:
    bool findAndAtack(std::shared_ptr<Map> map, std::function<void(const uint32_t, const uint32_t, const uint32_t)> &&callback) const override
    {
        std::vector<std::shared_ptr<IUnit>> enemies;
        map->scanAround(getId(), atackRadius, enemies);
        if (enemies.empty())
        {
            return false;
        }
        return meleeAtack(enemies, std::move(callback));
    }

    bool meleeAtack(const std::vector<std::shared_ptr<IUnit>> &enemies, std::function<void(const uint32_t, const uint32_t, const uint32_t)> &&callback) const
    {
        if (enemies.empty())
        {
            return false;
        }

        std::shared_ptr<IUnit> target;
        if (enemies.size() == 1)
        {
            target = enemies[0];
        }
        else
        {
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
                return false;
            }

            if (min_hp_units.size() == 1)
            {
                target = min_hp_units[0];
            }
            else
            {
                std::sort(min_hp_units.begin(), min_hp_units.end(), [](const std::shared_ptr<IUnit> &l, const std::shared_ptr<IUnit> &r)
                {
                    return l->getId() < r->getId();
                });
                target = min_hp_units[0];
            }
        }

        if (!target)
        {
            return false;
        }
        target->damage(_strength);
        callback(target->getId(), _strength, target->getHp());
        return true;
    }
};

} // namespace sw
