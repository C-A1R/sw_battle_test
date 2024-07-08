#include "Battle.h"

#include "BattleCore/Core.hpp"
#include "BattleActions/BattleAction_CreateMap.h"
#include "BattleActions/BattleAction_SpawnWarrior.h"
#include "BattleActions/BattleAction_SpawnArcher.h"
#include "BattleActions/BattleAction_March.h"
#include "BattleActions/BattleAction_Wait.h"

namespace sw
{

Battle::Battle()
    : _core{std::make_shared<Core>()}
{
}

void Battle::addCommand(io::CreateMap &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_CreateMap>(std::move(cmd), _core));
}

void Battle::addCommand(io::SpawnWarrior &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_SpawnWarrior>(std::move(cmd), _core));
}

void Battle::addCommand(io::SpawnArcher &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_SpawnArcher>(std::move(cmd), _core));
}

void Battle::addCommand(io::March &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_March>(std::move(cmd), _core));
}

void Battle::addCommand(io::Wait &&cmd)
{
    _startup_actions.emplace(std::make_unique<BattleAction_Wait>(std::move(cmd), _core));
}

void Battle::run()
{
    uint32_t tick = 1;
    /// create game in the first tick
    while(!_startup_actions.empty())
    {
        _startup_actions.front()->exec(tick);
        _startup_actions.pop();
    }

    if (!_core->isValid())
    {
        throw std::runtime_error("Error: game map is invalid");
    }

    /// main game loop
    while (true)
    {
        if (!_core->tick(tick++))
            break;
    }
}

} // namespace sw