#pragma once

#include "IUnit.hpp"

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
    ActionResult move(const uint32_t targetX, const uint32_t targetY) override
    {
        std::vector<uint32_t> enemies;
        _map->csanRadius(id(), atackRadius, enemies);
        if (enemies.empty())
        {
            return Unit::move(targetX, targetY);
        }
        return meleeAtack(enemies);
    }

    virtual ActionResult meleeAtack(const std::vector<uint32_t> &enemies)
    {
        if (enemies.empty())
        {
            return ActionResult::fail;
        }
        if (enemies.size() == 1)
        {
            uint32_t atackedEnemy = enemies[0];
        }



        return ActionResult::success;
    }
};

} // namespace sw
