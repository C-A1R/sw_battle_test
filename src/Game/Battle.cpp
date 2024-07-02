#include "Battle.hpp"

#include "IO/System/EventLog.hpp"
#include "IO/Events/UnitDied.hpp"

#include "BattleActions/BattleAction_CreateMap.h"
#include "BattleActions/BattleAction_SpawnWarrior.h"
#include "BattleActions/BattleAction_SpawnArcher.h"
#include "BattleActions/BattleAction_March.h"
#include "BattleActions/BattleAction_Wait.h"

namespace sw
{

Battle::Battle()
    : _map{std::make_shared<Map>()}
{
}

void Battle::addCommand(io::CreateMap &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_CreateMap>(std::move(cmd), _map));
}

void Battle::addCommand(io::SpawnWarrior &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_SpawnWarrior>(std::move(cmd), _map));
}

void Battle::addCommand(io::SpawnArcher &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_SpawnArcher>(std::move(cmd), _map));
}

void Battle::addCommand(io::March &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_March>(std::move(cmd), _map));
}

void Battle::addCommand(io::Wait &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_Wait>(std::move(cmd), _map));
}

void Battle::run()
{
    uint32_t tick = 1;
    /// to create game in the first tick
    while(!_startup_actions.empty())
    {
        _startup_actions.front()->exec(tick);
        _startup_actions.pop();
    }

    if (!_map || !_map->isValid())
    {
        throw std::runtime_error("Error: game map wasn't created");
    }
    if (!_map->hasUnits())
    {
        throw std::runtime_error(std::string("Error: there are no units in the game"));
    }

    /// main game loop
    const std::unique_ptr<UnitRegister> &units = _map->getUnits();
    bool noActionsExecuted = false;
    std::vector<uint32_t> toRemove;
    while (!noActionsExecuted)
    {
        noActionsExecuted = true;
        for(auto i = units->begin(); i != units->end(); ++i)
        {
            auto &unit = i->second;
            if (!unit->getHp())
            {
                toRemove.push_back(unit->getId());
                EventLog::instance().log(tick++, io::UnitDied{unit->getId()});
                continue;
            }

            if (unit->execNextAction(_map, tick))
            {
                ++tick;
                noActionsExecuted = false;
            }
        }

        for (const uint32_t id : toRemove)
        {
            _map->kill(id);
            units->remove(id);
        }
        toRemove.clear();
    }
}

} // namespace sw