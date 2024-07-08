#pragma once

#include "BattleActions/IBattleAction.h"

#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/SpawnWarrior.hpp"
#include "IO/Commands/SpawnArcher.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/Wait.hpp"

#include <memory>
#include <queue>

namespace sw
{

class Core;

class Battle
{
    std::shared_ptr<Core>                       _core;
    std::queue<std::unique_ptr<IBattleAction>>  _startup_actions;

public:
    Battle();

    void addCommand(io::CreateMap &&cmd);
    void addCommand(io::SpawnWarrior &&cmd);
    void addCommand(io::SpawnArcher &&cmd);
    void addCommand(io::March &&cmd);
    void addCommand(io::Wait &&cmd);

    void run();
};

} // namespace sw