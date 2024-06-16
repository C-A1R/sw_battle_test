#pragma once

#include <cstdint>
#include <queue>
#include <memory>

#include "IUnitAction.hpp"

namespace sw
{

/// @brief  Unit interface
class IUnit
{
public:
    IUnit() = default;
    virtual ~IUnit() = default;

    virtual uint32_t id() const = 0;
    virtual void march(uint32_t targetX, uint32_t targetY) = 0;
};

/// @brief  Unit base class
class Unit : public IUnit
{
    const uint32_t  _id {0};
    uint32_t        _hp {0};
    std::queue<std::unique_ptr<IUnitAction>> _actions;

public:
    Unit(const uint32_t id, const uint32_t hp)
        : _id{id}
        , _hp{hp}
    {
    }
    ~Unit() override = default;

protected:
    uint32_t id() const override { return _id; }
    void march(uint32_t targetX, uint32_t targetY) override
    {
        _actions.emplace(std::make_unique<MarchAction>(targetX, targetY));
    }
};

} // namespace sw
