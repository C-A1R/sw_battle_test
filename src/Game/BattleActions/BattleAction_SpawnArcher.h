#pragma once

#include "IBattleAction.h"

#include "../BattleCore/Core.hpp"
#include "../Units/Archer.hpp"
#include "../../IO/Events/UnitSpawned.hpp"

namespace sw
{

class BattleAction_SpawnArcher : public IBattleAction
{
    io::SpawnArcher         _cmd;
    std::shared_ptr<Core>   _core;

public:
    BattleAction_SpawnArcher(io::SpawnArcher &&cmd, const std::shared_ptr<Core> &core)
        : _cmd{std::move(cmd)}
        , _core{core}
    {
    }
    
protected:
    void exec(const uint32_t tick) override
    {
        if (!_core->isValid())
        {
            return;
        }

        if (_core->spawn(std::make_shared<Archer>(_cmd.unitId, _cmd.hp, _cmd.strength, _cmd.range, _cmd.agility), _cmd.x, _cmd.y))
        {
            EventLog::instance().log(tick, io::UnitSpawned{_cmd.unitId, "Archer", _cmd.x, _cmd.y });
        }
    }
};

} // namespace sw
