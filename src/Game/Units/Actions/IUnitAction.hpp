#pragma once

#include <cstdint>
#include <memory>
#include <queue>

#include <IO/System/PrintDebug.hpp>
#include <IO/System/EventLog.hpp>

namespace sw
{

class Map;

class IUnitAction
{
public:
    IUnitAction() = default;
    virtual ~IUnitAction() = default;
    virtual bool exec(const int32_t) = 0;
    virtual void addAction(const std::shared_ptr<IUnitAction> &act) = 0;
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
    void addAction(const std::shared_ptr<IUnitAction> &/*act*/) final
    {
    }
};

/// @brief The class unites several actions. It tries to execute the actions consistently while one of them will be executed.
/// Only one or nothing will be executed
class MultiAction : public IUnitAction
{
    std::queue<std::shared_ptr<IUnitAction>> _actions;
public:
    MultiAction() = default;
    void addAction(const std::shared_ptr<IUnitAction> &act) override
    {
        _actions.emplace(std::move(act));
    }
    bool exec(const int32_t tick) override 
    {
        while (!_actions.empty())
        {
            auto &act = _actions.front();
            if (act->exec(tick)) /// @todo act type
            {
                return true;
            }
            _actions.pop();
        }
        return false;
    }
};

} // namespace sw
