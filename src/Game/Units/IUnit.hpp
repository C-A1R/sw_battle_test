#pragma once

#include <cstdint>
#include <queue>
#include <memory>

#include "Actions/IUnitAction.hpp"

namespace sw
{
enum class ActionResult
{
    success,
    fail,
    impossible,         ///< something prevents
    run_another_action  ///< it's need to run more priority action
};

/// @brief  Unit interface
class IUnit
{
public:
    IUnit() = default;
    virtual ~IUnit() = default;

    virtual void setMap(const std::shared_ptr<IMap> &map) = 0;

    virtual uint32_t id() const = 0;

    virtual void addAction(std::unique_ptr<IUnitAction> &&action) = 0;
    virtual bool execNextAction(const int32_t tick) = 0;

    virtual ActionResult spawn(uint32_t x, uint32_t y) = 0;
    virtual ActionResult move(const uint32_t targetX, const uint32_t targetY) = 0;
};

/// @brief  Unit base class
class Unit : public IUnit
{
    const uint32_t          _id {0};
    uint32_t                _hp {0};
    std::queue<std::unique_ptr<IUnitAction>> _actions;
protected:
    std::shared_ptr<IMap>   _map;

public:
    Unit(const uint32_t id, const uint32_t hp)
        : _id{id}
        , _hp{hp}
    {
    }
    ~Unit() override = default;

protected:
    uint32_t id() const override { return _id; }
    void setMap(const std::shared_ptr<IMap> &map) override { _map = map; }

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

    ActionResult spawn(uint32_t x, uint32_t y) override 
    {
        if (!_map->spawn(_id, x, y))
        {
            return ActionResult::fail;
        }
        return ActionResult::success;
    }

    ActionResult move(const uint32_t targetX, const uint32_t targetY) override
    {
        if (!_map->move(id(), targetX, targetY))
        {
            return ActionResult::impossible; /// @todo impossible
        }
        return ActionResult::success;
    }
};

} // namespace sw
