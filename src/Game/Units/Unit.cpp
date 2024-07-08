#include "Unit.hpp"

#include "UnitActions/IUnitAction.hpp"

namespace sw
{

Unit::Unit(const uint32_t id, const uint32_t hp)
    : _id{id}
    , _hp{hp}
{
}

uint32_t Unit::getId() const
{
    return _id;
}

uint32_t Unit::getHp() const
{
    return _hp;
}

void Unit::addAction(const std::shared_ptr<IUnitAction> &action)
{
    _actions.emplace(action);
}

bool Unit::execNextAction(const std::shared_ptr<Core> &core, const int32_t tick)
{
    if (findAndAtack(core, tick))
    {
        return true;
    }

    if (_actions.empty())
    {
        return false;
    }
    auto &act = _actions.front();
    const auto result = act->exec(tick);
    if (result != ActionResult::skip && result != ActionResult::impossible)
    {
        _actions.pop();
    }
    return true;
}

uint32_t Unit::damage(uint32_t points)
{
    if (points > _hp)
    {
        points = _hp;
        _hp = 0;
    }
    else
    {
        _hp -= points;
    }
    return points;
}

} // namespace sw
