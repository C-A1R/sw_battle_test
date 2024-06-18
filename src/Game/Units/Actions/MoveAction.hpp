#pragma once

#include <cstdint>
#include <functional>

#include "IUnitAction.hpp"
#include "../../Maps/IMap.hpp"
#include "../IUnit.hpp"

namespace sw
{

class MarchStartAction : public IUnitAction
{
    std::function<void(const int32_t)> _callback;
public:
    MarchStartAction(std::function<void(const int32_t)> &&callback)
        : _callback{std::move(callback)}
    {
    }
    bool exec(const int32_t tick) override 
    {
        _callback(tick);
        return true;
    }
};

class MoveAction : public IUnitAction
{
    std::shared_ptr<IUnit> _unit;
    uint32_t _targetX {0};
    uint32_t _targetY {0};
    std::function<void(const int32_t, const int32_t, const int32_t, const int32_t)>  _callback;
public:
    MoveAction(const std::shared_ptr<IUnit> &unit, const uint32_t targetX, const uint32_t targetY
              , std::function<void(const int32_t, const int32_t, const int32_t, const int32_t)> &&callback) 
        : _unit{unit}
        , _targetX{std::move(targetX)}
        , _targetY{std::move(targetY)}
        , _callback{std::move(callback)}
    {
    }
    bool exec(const int32_t tick) override
    {
        if (_unit->move(_targetX, _targetY) == ActionResult::success)
        {
            _callback(tick, _unit->id(), _targetX, _targetY);
            return true;
        }
        return false;
    }
};

class MarchEndAction : public IUnitAction
{
    std::function<void(const int32_t)> _callback;
public:
    MarchEndAction(std::function<void(const int32_t)> &&callback) 
        : _callback{std::move(callback)}
    {
    }
    bool exec(const int32_t tick) override
    {
        _callback(tick);
        return true;
    }
};

} // namespace sw
