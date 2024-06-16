#pragma once

#include <cstdint>

namespace sw
{

class IUnitAction
{
public:
    IUnitAction() = default;
    virtual ~IUnitAction() = default;
    virtual void exec() = 0;
};

class MarchAction : public IUnitAction
{
    uint32_t _targetX {0};
    uint32_t _targetY {0};
public:
    MarchAction(const uint32_t targetX, const uint32_t targetY)
        : _targetX{targetX}
        , _targetY{targetY}
    {
    }
protected:
    void exec() override
    {
        
    }
};

} // namespace sw
