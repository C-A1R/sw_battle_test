#pragma once

#include <cstdint>
#include <queue>
#include <memory>
#include <functional>

// #include "../Maps/Map.hpp"
#include "Actions/IUnitAction.hpp"

#include "Actions/MoveAction.hpp"

namespace sw
{


/// @brief  Unit interface
class IUnit
{
public:
    IUnit() = default;
    virtual ~IUnit() = default;

    virtual uint32_t getId() const = 0;
    virtual uint32_t getHp() const = 0;

    virtual void addAction(const std::shared_ptr<IUnitAction> &action) = 0;
    virtual bool hasNextAction() const = 0;
    virtual bool execNextAction(const int32_t tick) = 0;

    virtual bool findAndAtack(const std::shared_ptr<Map> &map, std::tuple<uint32_t, uint32_t, uint32_t> &t) const = 0;
    virtual uint32_t damage(const uint32_t points) = 0;
};

/// @brief  Unit base class
class Unit : public IUnit
{
    const uint32_t  _id {0};
    uint32_t        _hp {0};
    std::queue<std::shared_ptr<IUnitAction>> _actions;

public:
    Unit(const uint32_t id, const uint32_t hp)
        : _id{id}
        , _hp{hp}
    {
    }
    ~Unit() override = default;

protected:
    uint32_t getId() const override { return _id; }
    uint32_t getHp() const override { return _hp; }

    void addAction(const std::shared_ptr<IUnitAction> &action) override
    {
        _actions.emplace(action);
    }

    bool hasNextAction() const override { return !_actions.empty(); }

    bool execNextAction(const int32_t tick) override
    {
        if (_actions.empty())
        {
            return false;
        }
        auto &act = _actions.front();
        if (const auto result = act->exec(tick);
            result != ActionResult::skip && result != ActionResult::impossible)
        {
            _actions.pop();
        }
        return true;
    }

    uint32_t damage(uint32_t points) override
    {
        if (points > _hp)
        {
            points = _hp;
            _hp = 0;
        }
        else
        {
            _hp -= points;
        }
        return points;
    }
};

} // namespace sw
