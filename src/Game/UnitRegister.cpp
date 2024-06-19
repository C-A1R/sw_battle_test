#include "UnitRegister.hpp"

#include "Units/IUnit.hpp"

namespace sw
{

void UnitRegister::insert(const std::shared_ptr<IUnit> &unit)
{
    _units_by_order.emplace(++_order, unit);
    _orders_by_id.emplace(unit->getId(), _order);
}

bool UnitRegister::empty() const
{
    return _units_by_order.empty() || _orders_by_id.empty();
}

std::shared_ptr<IUnit> UnitRegister::find(const std::uint32_t id) const
{
    auto o_it = _orders_by_id.find(id);
    if (o_it == _orders_by_id.cend())
    {
        return nullptr;
    }
    auto u_it = _units_by_order.find(o_it->second);
    if (u_it == _units_by_order.cend())
    {
        return nullptr;
    }
    return u_it->second;
}

void UnitRegister::remove(const std::uint32_t id)
{
    auto o_it = _orders_by_id.find(id);
    if (o_it == _orders_by_id.cend())
    {
        return;
    }
    _units_by_order.erase(o_it->second);
    _orders_by_id.erase(id);
}

} // namespace sw