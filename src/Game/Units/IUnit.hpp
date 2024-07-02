#pragma once

#include <cstdint>
#include <memory>

namespace sw
{

class Map;
class IUnitAction;

/// @brief  Unit interface
class IUnit
{
public:
    IUnit() = default;
    virtual ~IUnit() = default;

    virtual uint32_t getId() const = 0;
    virtual uint32_t getHp() const = 0;

    virtual void addAction(const std::shared_ptr<IUnitAction> &action) = 0;
    virtual bool execNextAction(const std::shared_ptr<Map> &map, const int32_t tick) = 0;

    virtual bool findAndAtack(const std::shared_ptr<Map> &map) const = 0;
    virtual uint32_t damage(const uint32_t points) = 0;
};

} // namespace sw
