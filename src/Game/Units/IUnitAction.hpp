#pragma once

#include <cstdint>

namespace sw
{

class IUnitAction
{
public:
    IUnitAction() = default;
    virtual ~IUnitAction() = default;
    virtual void exec(const int32_t) = 0;
};

class MarchStartAction : public IUnitAction
{
    std::function<void(const int32_t)> _callback;
public:
    MarchStartAction(std::function<void(const int32_t)> &&callback)
        : _callback{std::move(callback)}
    {
    }
    void exec(const int32_t tick) override 
    {
        _callback(tick);
    }
};

class MoveAction : public IUnitAction
{
    std::shared_ptr<IMap> _map;
    uint32_t _unitId {0};
    uint32_t _targetX {0};
    uint32_t _targetY {0};
    std::function<void(const int32_t, const int32_t, const int32_t, const int32_t)>  _callback;
public:
    MoveAction(const std::shared_ptr<IMap> &map, uint32_t unitId, const uint32_t targetX, const uint32_t targetY
              , std::function<void(const int32_t, const int32_t, const int32_t, const int32_t)> &&callback) 
        : _map{map}
        , _unitId{unitId}
        , _targetX{std::move(targetX)}
        , _targetY{std::move(targetY)}
        , _callback{std::move(callback)}
    {
    }
    void exec(const int32_t tick) override
    {
        if (_map->move(_unitId, _targetX, _targetY))
        {
            _callback(tick, _unitId, _targetX, _targetY);
        }
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
    void exec(const int32_t tick) override
    {
        _callback(tick);
    }
};

} // namespace sw
