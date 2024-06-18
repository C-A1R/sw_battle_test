#include "Battle.hpp"

namespace sw
{

void Battle::UnitRegister::insert(const std::shared_ptr<IUnit> &unit)
{
    _units_by_time.emplace(++_count, unit);
    _units_by_id.emplace(unit->id(), unit);
}

bool Battle::UnitRegister::empty() const
{
    return _units_by_time.empty() || _units_by_id.empty();
}

std::shared_ptr<IUnit> Battle::UnitRegister::find(const std::uint32_t id)
{
    auto it = _units_by_id.find(id);
    if (it == _units_by_id.cend())
    {
        return nullptr;
    }
    return it->second;
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
    if (_units.empty())
    {
        throw std::runtime_error(std::string("Error: there are no units in the game"));
    }

    /// main game loop
    bool noActionsExecuted = false;
    while (!noActionsExecuted)
    {
        noActionsExecuted = true;
        for(auto i = _units.begin(); i != _units.end(); ++i)
        {
            if (i->second->execNextAction(tick))
            {
                ++tick;
                noActionsExecuted = false;
            }
        }
    }
}

} // namespace sw