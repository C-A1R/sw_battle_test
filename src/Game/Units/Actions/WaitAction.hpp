#pragma once

#include "IUnitAction.hpp"

namespace sw
{

class WaitAction : public IUnitAction
{
public:
    WaitAction() = default;
protected:
    ActionResult exec(const int32_t tick) final
    {
        return ActionResult::success;
    }
};

} // namespace sw
