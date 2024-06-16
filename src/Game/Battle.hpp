#pragma once

#include <memory>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <functional>

#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/SpawnWarrior.hpp"
#include "IO/Commands/SpawnArcher.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/Wait.hpp"

#include "Maps/RectangleMap.hpp"
#include "Units/Warrior.hpp"
#include "Units/Archer.hpp"

#include <IO/System/PrintDebug.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/UnitSpawned.hpp>


namespace sw
{

class Battle
{
    class UnitRegister
    {
        std::unordered_map<std::time_t, std::shared_ptr<IUnit>>     _units_by_time;
        std::unordered_map<std::uint32_t, std::shared_ptr<IUnit>>   _units_by_id;
    public:
        UnitRegister() = default;
        void insert(const std::shared_ptr<IUnit> &unit);
        bool empty() const;
        std::shared_ptr<IUnit> find(std::uint32_t id);
    };

    std::shared_ptr<IMap>   _map;
    UnitRegister            _units;
    std::queue<std::function<void()>> _global_actions;
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
        _global_actions.emplace([cmd = std::move(cmd), this]() mutable
        {
            if (!cmd.width || !cmd.height)
            {
                return;
            }
            _map.reset(new RectangleMap(cmd.width, cmd.height));
            _eventLog.log(1, io::MapCreated{cmd.width, cmd.height});
        });
    }
    template<>
    void addCommand(io::SpawnWarrior &&cmd)
    {
        _global_actions.emplace([cmd = std::move(cmd), this]() mutable
        {
            if (!_map)
            {
                return;
            }
            std::shared_ptr<IUnit> unit = std::make_shared<Warrior>(cmd.unitId, cmd.hp, cmd.strength);
            if (_map->spawn(unit, cmd.x, cmd.y))
            {
                _units.insert(unit);
            }
            _eventLog.log(1, io::UnitSpawned{unit->id(), "Warrior", cmd.x, cmd.y });
        });
    }
    template<>
    void addCommand(io::SpawnArcher &&cmd)
    {
        _global_actions.emplace([cmd = std::move(cmd), this]() mutable
        {
            if (!_map)
            {
                return;
            }
            std::shared_ptr<IUnit> unit = std::make_shared<Archer>(cmd.unitId, cmd.hp, cmd.strength, cmd.range, cmd.agility);
            if (_map->spawn(unit, cmd.x, cmd.y))
            {
                _units.insert(unit);
            }
            _eventLog.log(1, io::UnitSpawned{unit->id(), "Archer", cmd.x, cmd.y });
        });
    }
    template<>
    void addCommand(io::March &&cmd)
    {
        auto unit = _units.find(cmd.unitId);
        if (!unit)
        {
            return;
        }
        unit->march(cmd.targetX, cmd.targetY);

    }
    template<>
    void addCommand(io::Wait &&cmd)
    {

    }

    void run();
};

} // namespace sw