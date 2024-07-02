#include "Battle.hpp"

#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>

namespace sw
{

Battle::Battle()
    : _units{std::make_shared<UnitRegister>()}
    , _eventLog{std::make_shared<EventLog>()}
{
}

void Battle::addCommand(io::CreateMap &&cmd)
{
    _startup_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick)
    {
        if (!cmd.width || !cmd.height)
        {
            return;
        }
        if (_map)
        {
            // map has been already created
            return;
        }
        _map.reset(new Map(_units, cmd.width, cmd.height));
        _eventLog->log(tick, io::MapCreated{cmd.width, cmd.height});
    });
}

void Battle::addCommand(io::SpawnWarrior &&cmd)
{
    _startup_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick)
    {
        if (!_map)
        {
            return;
        }
        if (_map->spawn(cmd.unitId, cmd.x, cmd.y))
        {
            _units->insert(std::make_shared<Warrior>(cmd.unitId, cmd.hp, cmd.strength));
        }
        _eventLog->log(tick, io::UnitSpawned{cmd.unitId, "Warrior", cmd.x, cmd.y });
    });
}

void Battle::addCommand(io::SpawnArcher &&cmd)
{
    _startup_actions.emplace([cmd = std::move(cmd), this](uint32_t &tick)
    {
        if (!_map)
        {
            return;
        }

        std::shared_ptr<IUnit> unit = std::make_shared<Archer>(cmd.unitId, cmd.hp, cmd.strength, cmd.range, cmd.agility);
        if (_map->spawn(cmd.unitId, cmd.x, cmd.y))
        {
            _units->insert(unit);
        }
        _eventLog->log(tick, io::UnitSpawned{cmd.unitId, "Archer", cmd.x, cmd.y });
    });
}

void Battle::addCommand(io::March &&cmd)
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

void Battle::addCommand(io::Wait &&cmd)
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

void Battle::run()
{
    uint32_t tick = 1;
    /// to create game in the first tick
    while(!_startup_actions.empty())
    {
        _startup_actions.front()(tick);
        _startup_actions.pop();
    }

    if (!_map )
    {
        throw std::runtime_error("Error: game map wasn't created");
    }
    if (!_map->isValid())
    {
        throw std::runtime_error(std::string("Error: game map is invalid"));
    }
    if (_units->empty())
    {
        throw std::runtime_error(std::string("Error: there are no units in the game"));
    }

    /// main game loop
    bool noActionsExecuted = false;
    std::vector<uint32_t> toRemove;
    while (!noActionsExecuted)
    {
        noActionsExecuted = true;
        for(auto i = _units->begin(); i != _units->end(); ++i)
        {
            auto &unit = i->second;
            if (!unit->getHp())
            {
                toRemove.push_back(unit->getId());
                _eventLog->log(tick++, io::UnitDied{unit->getId()});
                continue;
            }

            if (!unit->hasNextAction())
            {
                std::tuple<uint32_t, uint32_t, uint32_t> t;
                if (unit->findAndAtack(_map, t))
                {
                    uint32_t tId {0};
                    uint32_t dam {0};
                    uint32_t tHp {0};
                    std::tie(tId, dam, tHp) = t;
                    _eventLog->log(tick++, io::UnitAttacked{unit->getId(), tId, dam, tHp});
                }
                continue;
            }

            unit->execNextAction(tick++);
            noActionsExecuted = false;
        }

        for (const uint32_t id : toRemove)
        {
            _map->kill(id);
            _units->remove(id);
        }
        toRemove.clear();
    }
}

void Battle::splitMarch(io::March &&cmd, std::vector<std::shared_ptr<IUnitAction>> &actions)
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
    while (!(tmpPoint.x == cmd.targetX && tmpPoint.y == cmd.targetY))
    {
        _doStep();
        actions.emplace_back(std::make_shared<MoveAction>(_eventLog, _map, cmd.unitId, tmpPoint.x, tmpPoint.y));
    }
    //end march
    actions.emplace_back(std::make_shared<MarchEndAction>(_eventLog, _map, cmd.unitId));
}

} // namespace sw