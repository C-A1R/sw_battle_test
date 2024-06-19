#include "Battle.hpp"

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
                _eventLog->log(tick, io::UnitDied{unit->getId()});
                continue;
            }

            if (!unit->hasNextAction())
                continue;

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

} // namespace sw