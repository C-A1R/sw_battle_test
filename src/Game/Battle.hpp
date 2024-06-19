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
    Battle()
        : _units{std::make_shared<UnitRegister>()}
        , _eventLog{std::make_shared<EventLog>()}
    {
    }

    template<typename TCommand>
    void addCommand(TCommand &&cmd)
    {
    }
    template<>
    void addCommand(io::CreateMap &&cmd)
    {
        _startup_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick)
        {
            if (!cmd.width || !cmd.height)
            {
                return;
            }
            if (_map)
            {
                // map is already created
                return;
            }
            _map.reset(new Map(_units, cmd.width, cmd.height));
            _eventLog->log(tick, io::MapCreated{cmd.width, cmd.height});
        });
    }
    template<>
    void addCommand(io::SpawnWarrior &&cmd)
    {
        _startup_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick)
        {
            if (!_map)
            {
                return;
            }
            std::shared_ptr<IUnit> unit = std::make_shared<Warrior>(cmd.unitId, cmd.hp, cmd.strength);
            if (_map->spawn(cmd.unitId, cmd.x, cmd.y))
            {
                _units->insert(unit);
            }
            _eventLog->log(tick, io::UnitSpawned{cmd.unitId, "Warrior", cmd.x, cmd.y });
        });
    }
    template<>
    void addCommand(io::SpawnArcher &&cmd)
    {
        _startup_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick)
        {
            if (!_map)
            {
                return;
            }
            std::shared_ptr<IUnit> unit = std::make_shared<Warrior>(cmd.unitId, cmd.hp, cmd.strength);
            if (_map->spawn(cmd.unitId, cmd.x, cmd.y))
            {
                _units->insert(unit);
            }
            _eventLog->log(tick, io::UnitSpawned{cmd.unitId, "Archer", cmd.x, cmd.y });
        });
    }
    template<>
    void addCommand(io::March &&cmd)
    {
        _startup_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick) mutable
        {
            auto unit = _units->find(cmd.unitId);
            if (!unit)
            {
                return;
            }

            std::vector<std::shared_ptr<IUnitAction>> actions;
            splitMarch(std::move(cmd), actions);
            for (int i = 0; i < actions.size(); ++i)
            {
                unit->addAction(actions[i]);
            }
        });
    }
    template<>
    void addCommand(io::Wait &&cmd)
    {
        _startup_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick) mutable
        {
            for(auto i = _units->begin(); i != _units->end(); ++i)
            {
                std::shared_ptr<IUnitAction> act = std::make_shared<WaitAction>();
                for (int j = 0; j < cmd.ticks; ++j)
                {
                    i->second->addAction(std::move(act));
                }
            }
        });
    }

    void run();

private:
    /// @brief separate march to several moves
    void splitMarch(io::March &&cmd, std::vector<std::shared_ptr<IUnitAction>> &actions)
    {
        bool ok = true;
        const Point currentPoint = _map->getPoint(cmd.unitId, ok);
        if (!ok || (currentPoint.x == cmd.targetX && currentPoint.y == cmd.targetY))
        {
            return;
        }

        /// the longest march length
        uint32_t marchLen = std::max(currentPoint.x, cmd.targetX) - std::min(currentPoint.x, cmd.targetX)
                          + std::max(currentPoint.y, cmd.targetY) - std::min(currentPoint.y, cmd.targetY);
        actions.reserve(marchLen + 2);
        //start march
        actions.emplace_back(std::make_shared<MarchStartAction>(_eventLog, _map, cmd.unitId, cmd.targetX, cmd.targetY));
        Point tmpPoint = currentPoint;
        //move
        auto _doStep = [&tmpPoint, &cmd]()
        {
            if (tmpPoint.x < cmd.targetX)
            {
                ++tmpPoint.x;
            }
            else if (tmpPoint.x > cmd.targetX)
            {
                --tmpPoint.x;
            }
            
            if (tmpPoint.y < cmd.targetY)
            {
                ++tmpPoint.y;
            }
            else if (tmpPoint.y > cmd.targetY)
            {
                --tmpPoint.y;
            }
        };
        auto unit = _units->find(cmd.unitId);
        while (!(tmpPoint.x == cmd.targetX && tmpPoint.y == cmd.targetY))
        {
            _doStep();
            actions.emplace_back(std::make_shared<MoveAction>(_eventLog, _map, cmd.unitId, tmpPoint.x, tmpPoint.y));
        }
        //end march
        actions.emplace_back(std::make_shared<MarchEndAction>(_eventLog, _map, cmd.unitId));
    }

    template<typename T>
    T callback(T &&event)
    {
        return event;
    }
};

} // namespace sw