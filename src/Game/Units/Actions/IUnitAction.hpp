#pragma once

#include <cstdint>
#include <memory>
#include <queue>

#include <IO/System/PrintDebug.hpp>
#include <IO/System/EventLog.hpp>

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
    std::shared_ptr<EventLog>   _eventLog;
    std::shared_ptr<Map>        _map;
public:
    SingleAction(){}
    SingleAction(const std::shared_ptr<EventLog> &eventLog, const std::shared_ptr<Map> &map)
        : _eventLog{eventLog}
        , _map{map}
    {
    }
};

} // namespace sw
