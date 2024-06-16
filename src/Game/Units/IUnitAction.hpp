#pragma once

#include <cstdint>

namespace sw
{

class IUnitAction
{
public:
    IUnitAction() = default;
    virtual ~IUnitAction() = default;
    virtual void exec(const int32_t tick) = 0;
};

class MarchAction : public IUnitAction
{
    uint32_t _targetX {0};
    uint32_t _targetY {0};
    std::function<void(const int32_t tick)> _callback;
public:
    MarchAction(const uint32_t targetX, const uint32_t targetY, std::function<void(const int32_t tick)> &&callback)
        : _targetX{targetX}
        , _targetY{targetY}
        , _callback{std::move(callback)}
    {
    }
protected:
    void exec(const int32_t tick) override
    {
        _callback(tick);
    }
};

} // namespace sw
