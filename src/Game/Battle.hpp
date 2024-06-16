#pragma once

#include <memory>
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

#include <IO/System/PrintDebug.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/Events/MarchStarted.hpp>


namespace sw
{

class Battle
{
    class UnitRegister
    {
        std::unordered_map<std::uint32_t, std::shared_ptr<IUnit>>   _units_by_time;
        std::unordered_map<std::uint32_t, std::shared_ptr<IUnit>>   _units_by_id;
        std::uint32_t _count {0};
    public:
        UnitRegister() = default;
        void insert(const std::shared_ptr<IUnit> &unit);
        bool empty() const;
        std::shared_ptr<IUnit> find(std::uint32_t id);

        auto begin() { return _units_by_time.begin(); }
        auto end() { return _units_by_time.end(); }
    };

    std::shared_ptr<IMap>   _map;
    UnitRegister            _units;
    std::queue<std::function<void(uint32_t &)>> _global_actions;
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
        _global_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick) mutable
        {
            if (!cmd.width || !cmd.height)
            {
                return;
            }
            _map.reset(new RectangleMap(cmd.width, cmd.height));
            _eventLog.log(tick, io::MapCreated{cmd.width, cmd.height});
        });
    }
    template<>
    void addCommand(io::SpawnWarrior &&cmd)
    {
        _global_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick) mutable
        {
            if (!_map)
            {
                return;
            }
            std::shared_ptr<IUnit> unit = std::make_shared<Warrior>(cmd.unitId, cmd.hp, cmd.strength);
            unit->setMap(_map);
            if (unit->spawn(cmd.x, cmd.y))
            {
                _units.insert(unit);
            }
            _eventLog.log(tick, io::UnitSpawned{unit->id(), "Warrior", cmd.x, cmd.y });
        });
    }
    template<>
    void addCommand(io::SpawnArcher &&cmd)
    {
        _global_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick) mutable
        {
            if (!_map)
            {
                return;
            }
            std::shared_ptr<IUnit> unit = std::make_shared<Archer>(cmd.unitId, cmd.hp, cmd.strength, cmd.range, cmd.agility);
            unit->setMap(_map);
            if (unit->spawn(cmd.x, cmd.y))
            {
                _units.insert(unit);
            }
            _eventLog.log(tick, io::UnitSpawned{unit->id(), "Archer", cmd.x, cmd.y });
        });
    }
    template<>
    void addCommand(io::March &&cmd)
    {
        _global_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick) mutable
        {
            auto unit = _units.find(cmd.unitId);
            if (!unit)
            {
                return;
            }
            unit->march(cmd.targetX, cmd.targetY, [this](const int32_t tick) mutable
            {
                _eventLog.log(tick, io::MarchStarted{ 1, 5, 3, 7, 9 });
            });/// @todo
        });
    }
    template<>
    void addCommand(io::Wait &&cmd)
    {
        /// @todo
    }

    void run();
};

} // namespace sw