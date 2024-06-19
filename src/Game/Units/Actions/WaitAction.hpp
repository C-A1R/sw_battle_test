#pragma once

#include "IUnitAction.hpp"

namespace sw
{

class WaitAction : public IUnitAction
{
public:
    WaitAction() = default;
protected:
    bool exec(const int32_t tick) final
    {
        return true;
    }
};

} // namespace sw
