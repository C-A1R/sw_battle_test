#pragma once

#include <memory>
#include <map>
#include <unordered_map>
#include <queue>
#include <functional>

#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/SpawnWarrior.hpp"
#include "IO/Commands/SpawnArcher.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/Wait.hpp"

#include "Maps/RectangleMap.hpp"
#include "Units/Warrior.hpp"
#include "Units/Archer.hpp"

#include "Units/Actions/MoveAction.hpp"

#include <IO/System/PrintDebug.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/MarchEnded.hpp>



namespace sw
{

class Battle
{
    class UnitRegister
    {
        std::map<std::uint32_t, std::shared_ptr<IUnit>>             _units_by_time;
        std::unordered_map<std::uint32_t, std::shared_ptr<IUnit>>   _units_by_id;
        std::uint32_t _count {0};
    public:
        UnitRegister() = default;
        void insert(const std::shared_ptr<IUnit> &unit);
        bool empty() const;
        std::shared_ptr<IUnit> find(const std::uint32_t id);

        auto begin() { return _units_by_time.begin(); }
        auto end() { return _units_by_time.end(); }
    };

    std::shared_ptr<IMap>   _map;
    UnitRegister            _units;
    std::queue<std::function<void(uint32_t &)>> _startup_actions; ///< actions for creating the game
    EventLog                _eventLog;

public:
    Battle() = default;

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
            _map.reset(new RectangleMap(cmd.width, cmd.height));
            _eventLog.log(tick, io::MapCreated{cmd.width, cmd.height});
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
            unit->setMap(_map);
            if (unit->spawn(cmd.x, cmd.y) == ActionResult::success)
            {
                _units.insert(unit);
            }
            _eventLog.log(tick, io::UnitSpawned{cmd.unitId, "Warrior", cmd.x, cmd.y });
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
            unit->setMap(_map);
            if (unit->spawn(cmd.x, cmd.y) == ActionResult::success)
            {
                _units.insert(unit);
            }
            _eventLog.log(tick, io::UnitSpawned{cmd.unitId, "Archer", cmd.x, cmd.y });
        });
    }
    template<>
    void addCommand(io::March &&cmd)
    {
        _startup_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick) mutable
        {
            auto unit = _units.find(cmd.unitId);
            if (!unit)
            {
                return;
            }

            std::vector<std::unique_ptr<IUnitAction>> actions;
            splitMarch(std::move(cmd), actions);
            for (int i = 0; i < actions.size(); ++i)
            {
                unit->addAction(std::move(actions[i]));
            }
        });
    }
    template<>
    void addCommand(io::Wait &&cmd)
    {
        /// @todo
    }

    void run();

private:
    /// @brief separate march to several moves
    void splitMarch(io::March &&cmd, std::vector<std::unique_ptr<IUnitAction>> &actions)
    {
        bool ok = true;
        const PlaneCoordinnates currentCoords = _map->getCoordinnates(cmd.unitId, ok);
        if (!ok || (currentCoords._x == cmd.targetX && currentCoords._y == cmd.targetY))
        {
            return;
        }

        /// the longest march length
        uint32_t marchLen = std::max(currentCoords._x, cmd.targetX) - std::min(currentCoords._x, cmd.targetX)
                            + std::max(currentCoords._y, cmd.targetY) - std::min(currentCoords._y, cmd.targetY);
        actions.reserve(marchLen + 2);
        //start march
        actions.push_back(std::make_unique<MarchStartAction>([this, cmd, currentCoords](const int32_t tick)
        {
            _eventLog.log(tick, io::MarchStarted{ cmd.unitId, currentCoords._x, currentCoords._y
                                                    , cmd.targetX, cmd.targetY });
        }));
        PlaneCoordinnates tmpCoords = currentCoords;
        //move
        auto _doStep = [&tmpCoords, &cmd]()
        {
            if (tmpCoords._x < cmd.targetX)
            {
                ++tmpCoords._x;
            }
            else if (tmpCoords._x > cmd.targetX)
            {
                --tmpCoords._x;
            }
            
            if (tmpCoords._y < cmd.targetY)
            {
                ++tmpCoords._y;
            }
            else if (tmpCoords._y > cmd.targetY)
            {
                --tmpCoords._y;
            }
        };
        auto unit = _units.find(cmd.unitId);
        while (!(tmpCoords._x == cmd.targetX && tmpCoords._y == cmd.targetY))
        {
            _doStep();
            actions.push_back(std::make_unique<MoveAction>(unit, tmpCoords._x, tmpCoords._y
                , [this](const int32_t tick, const uint32_t unitId, const uint32_t targetX, const uint32_t targetY)
            {
                _eventLog.log(tick, io::UnitMoved{ unitId, targetX, targetY});
            }));
        }
        //end march
        actions.push_back(std::make_unique<MarchEndAction>([this, cmd, tmpCoords](const int32_t tick)
        {
            _eventLog.log(tick, io::MarchEnded{ cmd.unitId, tmpCoords._x, tmpCoords._y });
        }));
    }
};

} // namespace sw