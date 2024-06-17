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

    virtual void addAction(std::unique_ptr<IUnitAction> &&action) = 0;
    virtual bool execNextAction(const int32_t tick) = 0;
};

/// @brief  Unit base class
class Unit : public IUnit
{
    const uint32_t          _id {0};
    uint32_t                _hp {0};
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

    void addAction(std::unique_ptr<IUnitAction> &&action) override
    {
        _actions.emplace(std::move(action));
    }

    /// @return true if any action was executed
    bool execNextAction(const int32_t tick) override
    {
        if (_actions.empty())
        {
            return false;
        }

        auto &act = _actions.front();
        act->exec(tick);
        _actions.pop();
        return true;
    }
};

} // namespace sw
