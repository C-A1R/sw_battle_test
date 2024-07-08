#pragma once

#include "IBattleAction.h"

#include "../BattleCore/Core.hpp"
#include "../../IO/System/EventLog.hpp"
#include "../../IO/Events/MapCreated.hpp"

namespace sw
{

class BattleAction_CreateMap : public IBattleAction
{
    io::CreateMap           _cmd;
    std::shared_ptr<Core>   _core;
public:
    BattleAction_CreateMap(io::CreateMap &&cmd, const std::shared_ptr<Core> &core)
        : _cmd{std::move(cmd)}
        , _core{core}
    {
    }
protected:
    void exec(const uint32_t tick) override
    {
        if (!_cmd.width || !_cmd.height || !_core)
        {
            return;
        }

        _core->initMap(_cmd.width, _cmd.height);
        EventLog::instance().log(tick, io::MapCreated{_cmd.width, _cmd.height});
    }
};

} // namespace sw
