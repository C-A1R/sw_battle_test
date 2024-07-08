#pragma once

#include "IBattleAction.h"

#include "../BattleCore/Core.hpp"
#include "../Units/UnitActions/WaitAction.hpp"

namespace sw
{

class BattleAction_Wait : public IBattleAction
{
    io::Wait                    _cmd;
    std::shared_ptr<Core>   _core;

public:
    BattleAction_Wait(io::Wait &&cmd, const std::shared_ptr<Core> &core)
        : _cmd{std::move(cmd)}
        , _core{core}
    {
    }

protected:
    void exec(const uint32_t tick) override
    {
        // const std::unique_ptr<UnitRegister> &units = _map->getUnits();
        // for(auto i = units->begin(); i != units->end(); ++i)
        // {
        //     std::shared_ptr<IUnitAction> act = std::make_shared<WaitAction>();
        //     for (int j = 0; j < _cmd.ticks; ++j)
        //     {
        //         i->second->addAction(std::move(act));
        //     }
        // }
    }
};

} // namespace sw
