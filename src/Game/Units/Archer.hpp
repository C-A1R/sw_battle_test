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
    Archer(const uint32_t id, const uint32_t hp, const uint32_t strength, const uint32_t range, const uint32_t agility);
    ~Archer() override = default;

protected:
    bool findAndAtack(const std::shared_ptr<Core> &core, const int32_t tick) const override;
    bool rangeAtack(const std::shared_ptr<Core> &core, const std::vector<std::shared_ptr<IUnit>> &enemies, const int32_t tick) const;
    std::shared_ptr<IUnit> chooseTarget(const std::shared_ptr<Core> &core, const std::vector<std::shared_ptr<IUnit>> &enemies) const;
};

} // namespace sw