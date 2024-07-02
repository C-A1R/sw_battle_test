#pragma once

#include "IBattleAction.h"

#include "../Map.hpp"
#include "../Units/Archer.hpp"
#include "../../IO/Events/UnitSpawned.hpp"

namespace sw
{

class BattleAction_SpawnArcher : public IBattleAction
{
    io::SpawnArcher         _cmd;
    std::shared_ptr<Map>    _map;
public:
    BattleAction_SpawnArcher(io::SpawnArcher &&cmd, std::shared_ptr<Map> map)
        : _cmd{std::move(cmd)}
        , _map{map}
    {
    }
protected:
    void exec(const uint32_t tick) override
    {
        if (!_map || !_map->isValid())
        {
            return;
        }
        if (_map->spawn(std::make_shared<Archer>(_cmd.unitId, _cmd.hp, _cmd.strength, _cmd.range, _cmd.agility), _cmd.x, _cmd.y))
        {
            EventLog::instance().log(tick, io::UnitSpawned{_cmd.unitId, "Archer", _cmd.x, _cmd.y });
        }
    }
};

} // namespace sw
