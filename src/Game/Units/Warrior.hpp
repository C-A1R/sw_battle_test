#pragma once

#include "Unit.hpp"


namespace sw
{

class Warrior : public Unit
{
    uint32_t _strength {0};
    static const uint8_t _meleeAtackDistance {1};

public:
    Warrior(const uint32_t id, const uint32_t hp, const uint32_t strength);
    ~Warrior() override = default;

protected:
    bool findAndAtack(const std::shared_ptr<Core> &core, const int32_t tick) const override;
    bool meleeAtack(const std::vector<std::shared_ptr<IUnit>> &enemies, const int32_t tick) const;
    std::shared_ptr<IUnit> chooseTarget(const std::vector<std::shared_ptr<IUnit>> &enemies) const;
};

} // namespace sw
