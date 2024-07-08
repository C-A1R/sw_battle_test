#pragma once

#include "IUnit.hpp"

#include <queue>

namespace sw
{

class IUnitAction;

/// @brief  Unit base class
class Unit : public IUnit
{
    const uint32_t  _id {0};
    uint32_t        _hp {0};
    std::queue<std::shared_ptr<IUnitAction>> _actions;

public:
    Unit(const uint32_t id, const uint32_t hp);
    ~Unit() override = default;

protected:
    uint32_t getId() const final;
    uint32_t getHp() const final;

    void addAction(const std::shared_ptr<IUnitAction> &action) final;
    bool execNextAction(const std::shared_ptr<Core> &core, const int32_t tick) final;
    uint32_t damage(uint32_t points) final;
};

} // namespace sw
