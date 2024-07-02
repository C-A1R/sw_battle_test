#pragma once

#include "../../../IO/System/PrintDebug.hpp"

#include <cstdint>
#include <memory>

namespace sw
{

class Map;

enum class ActionResult
{
    success,
    fail,
    impossible, ///< if something prevents
    skip
};

class IUnitAction
{
public:
    IUnitAction() = default;
    virtual ~IUnitAction() = default;
    virtual ActionResult exec(const int32_t) = 0;
};

class SingleAction : public IUnitAction
{
protected:
    std::shared_ptr<Map> _map;
public:
    SingleAction(){}
    SingleAction(const std::shared_ptr<Map> &map)
        : _map{map}
    {
    }
};

} // namespace sw
