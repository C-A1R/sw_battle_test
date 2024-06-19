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
    while (!noActionsExecuted)
    {
        noActionsExecuted = true;
        for(auto i = _units->begin(); i != _units->end(); ++i)
        {
            auto &unit = i->second;
            if (!unit->getHp())
            {
                _map->kill(unit->getId());
                _units->remove(unit->getId());
                _eventLog->log(tick, io::UnitDied{unit->getId()});
            }
            if (!unit->hasNextAction())
                continue;
            unit->execNextAction(tick);
            ++tick;
            noActionsExecuted = false;
        }
    }
}

} // namespace sw