#pragma once

#include "Warrior.hpp"

namespace sw
{

class Archer : public Warrior
{
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
};

} // namespace sw