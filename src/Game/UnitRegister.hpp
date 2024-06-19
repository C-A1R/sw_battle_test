#pragma once

#include <cstdint>
#include <map>
#include <unordered_map>
#include <memory>

namespace sw
{

class IUnit;

class UnitRegister
{
    std::map<std::uint32_t, std::shared_ptr<IUnit>>     _units_by_order; ///< key:order, val:unit
    std::unordered_map<std::uint32_t, std::uint32_t>    _orders_by_id; ///< key:unitId, val:order
    std::uint32_t _order {0};
public:
    UnitRegister() = default;
    void insert(const std::shared_ptr<IUnit> &unit);
    bool empty() const;
    std::shared_ptr<IUnit> find(const std::uint32_t id) const;
    void remove(const std::uint32_t id);

    auto begin() { return _units_by_order.begin(); }
    auto end()   { return _units_by_order.end(); }
};

} // namespace sw