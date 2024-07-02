#pragma once

#include "IUnit.hpp"
#include "UnitActions/IUnitAction.hpp"

#include <queue>

namespace sw
{

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

    bool execNextAction(const std::shared_ptr<Map> &map, const int32_t tick) final
    {
        if (findAndAtack(map))
        {
            return true;
        }

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
