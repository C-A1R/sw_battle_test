#include "Battle.hpp"

namespace sw
{

using sc = std::chrono::system_clock;

void Battle::UnitRegister::insert(const std::shared_ptr<IUnit> &unit)
{
    _units_by_time.emplace(sc::to_time_t(sc::now()), unit);
    _units_by_id.emplace(unit->id(), unit);
}

bool Battle::UnitRegister::empty() const
{
    return _units_by_time.empty() || _units_by_id.empty();
}

std::shared_ptr<IUnit> Battle::UnitRegister::find(std::uint32_t id)
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
    // game creating
    while(!_global_actions.empty())
    {
        _global_actions.front()();
        _global_actions.pop();
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


}

} // namespace sw