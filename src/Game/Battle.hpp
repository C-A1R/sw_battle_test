#pragma once

#include <memory>
#include <queue>
#include <functional>

#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/SpawnWarrior.hpp"
#include "IO/Commands/SpawnArcher.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/Wait.hpp"

#include "UnitRegister.hpp"
#include "Map.hpp"
#include "Units/Warrior.hpp"
#include "Units/Archer.hpp"

#include "Units/Actions/MoveAction.hpp"
#include "Units/Actions/WaitAction.hpp"

#include <IO/System/PrintDebug.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/UnitSpawned.hpp>


namespace sw
{

class Battle
{
    std::shared_ptr<UnitRegister>   _units;
    std::shared_ptr<EventLog>       _eventLog;
    std::shared_ptr<Map>            _map;
    std::queue<std::function<void(uint32_t &)>> _startup_actions; ///< actions for creating the game

public:
    Battle();

    void addCommand(io::CreateMap &&cmd);
    void addCommand(io::SpawnWarrior &&cmd);
    void addCommand(io::SpawnArcher &&cmd);
    void addCommand(io::March &&cmd);
    void addCommand(io::Wait &&cmd);

    void run();

private:
    /// @brief separate march to several moves
    void splitMarch(io::March &&cmd, std::vector<std::shared_ptr<IUnitAction>> &actions);
};

} // namespace sw