#pragma once

#include "IBattleAction.h"

#include "../Map.hpp"
#include "../../IO/Events/MapCreated.hpp"

namespace sw
{

class BattleAction_CreateMap : public IBattleAction
{
    io::CreateMap           _cmd;
    std::shared_ptr<Map>    _map;
public:
    BattleAction_CreateMap(io::CreateMap &&cmd, const std::shared_ptr<Map> &map)
        : _cmd{std::move(cmd)}
        , _map{map}
    {
    }
protected:
    void exec(const uint32_t tick) override
    {
        if (!_cmd.width || !_cmd.height)
        {
            return;
        }
        
        if (_map && _map->isValid())
        {
            // map has been already created
            return;
        }
        _map->init(_cmd.width, _cmd.height);
        EventLog::instance().log(tick, io::MapCreated{_cmd.width, _cmd.height});
    }
};

} // namespace sw
