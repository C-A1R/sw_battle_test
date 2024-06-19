#include "Battle.hpp"

#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>

namespace sw
{

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
                    uint32_t d {0};
                    uint32_t tHp {0};
                    std::tie(tId, d, tHp) = t;
                    _eventLog->log(tick, io::UnitAttacked{unit->getId(), tId, d, tHp});
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
    auto unit = _units->find(cmd.unitId);
    while (!(tmpPoint.x == cmd.targetX && tmpPoint.y == cmd.targetY))
    {
        _doStep();
        actions.emplace_back(std::make_shared<MoveAction>(_eventLog, _map, cmd.unitId, tmpPoint.x, tmpPoint.y));
    }
    //end march
    actions.emplace_back(std::make_shared<MarchEndAction>(_eventLog, _map, cmd.unitId));
}

} // namespace sw