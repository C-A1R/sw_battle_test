#pragma once

#include "IUnit.hpp"

namespace sw
{

class Warrior : public Unit
{
    uint32_t _strength {0};
public:
    Warrior(const uint32_t id, const uint32_t hp, const uint32_t strength)
        : Unit(id, hp)
        , _strength{strength}
    {
    }
    ~Warrior() override = default;
};

} // namespace sw
