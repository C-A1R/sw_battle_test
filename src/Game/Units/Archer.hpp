#pragma once

#include "Warrior.hpp"

namespace sw
{

class Archer : public Warrior
{
    static const uint8_t _minShotDistance {2};
    uint32_t _range {0};
    uint32_t _agility {0};

public:
    Archer(const uint32_t id, const uint32_t hp, const uint32_t strength, const uint32_t range, const uint32_t agility)
        : Warrior(id, hp, strength)
        , _range{range}
        , _agility{agility}
    {
    }
    ~Archer() override = default;

protected:
    bool findAndAtack(const std::shared_ptr<Map> &map) const override
    {
        std::vector<std::shared_ptr<IUnit>> enemies;
        map->scanAround(getId(), _minShotDistance, _range, enemies);
        if (enemies.empty())
        {
            return Warrior::findAndAtack(map);
        }
        return rangeAtack(map, enemies);
    }

    bool rangeAtack(const std::shared_ptr<Map> &map, const std::vector<std::shared_ptr<IUnit>> &enemies) const
    {
        if (enemies.empty() || !map)
        {
            return false;
        }

        std::shared_ptr<IUnit> target = chooseTarget(map, enemies);
        if (!target)
        {
            return false;
        }
        const uint32_t harm = target->damage(_agility);
        EventLog::instance().log(0/*tick*/, io::UnitAttacked{getId(), target->getId(), harm, target->getHp()}); /// @todo tock
        return true;
    }

    std::shared_ptr<IUnit> chooseTarget(const std::shared_ptr<Map> &map, const std::vector<std::shared_ptr<IUnit>> &enemies) const
    {
        if (enemies.empty() || !map)
        {
            return nullptr;
        }

        if (enemies.size() == 1)
        {
            return enemies[0];
        }

        uint32_t distance = 0;
        bool ok = false;
        auto myPoint = map->getUnitPoint(getId(), ok);
        if (!ok)
        {
            return nullptr;
        }

        double minDist = std::numeric_limits<double>::max();
        std::unordered_map<uint32_t, double> distancesToThis;
        for (const auto &e : enemies)
        {
            bool ok = false;
            const Point &ePoint = map->getUnitPoint(e->getId(), ok);
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
};

} // namespace sw