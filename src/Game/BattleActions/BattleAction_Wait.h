#pragma once

#include "IBattleAction.h"

#include "../Map.hpp"
#include "../Units/Actions/WaitAction.hpp"

namespace sw
{

class BattleAction_Wait : public IBattleAction
{
    io::Wait                _cmd;
    std::shared_ptr<Map>    _map;
public:
    BattleAction_Wait(io::Wait &&cmd, std::shared_ptr<Map> map)
        : _cmd{std::move(cmd)}
        , _map{map}
    {
    }
protected:
    void exec(const uint32_t tick) override
    {
        for(auto i = _map->_units->begin(); i != _map->_units->end(); ++i)
        {
            std::shared_ptr<IUnitAction> act = std::make_shared<WaitAction>();
            for (int j = 0; j < _cmd.ticks; ++j)
            {
                i->second->addAction(std::move(act));
            }
        }
    }
};

} // namespace sw
