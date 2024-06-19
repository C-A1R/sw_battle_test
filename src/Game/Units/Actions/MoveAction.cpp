#include "MoveAction.hpp"

#include "../../Map.hpp"
#include "../IUnit.hpp"

#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/UnitAttacked.hpp>

namespace sw
{

MarchStartAction::MarchStartAction(const std::shared_ptr<EventLog> &eventLog
                    , const std::shared_ptr<Map> &map
                    , const uint32_t unitId
                    , const uint32_t targetX
                    , const uint32_t targetY)
    : SingleAction(eventLog, map)
    , _unitId{unitId}
    , _targetX{std::move(targetX)}
    , _targetY{std::move(targetY)}
{
}

ActionResult MarchStartAction::exec(const int32_t tick) 
{
    bool ok = false;
    auto unitPoint = _map->getPoint(_unitId, ok);
    if (!ok)
    {
        return ActionResult::fail;
    }
    _eventLog->log(tick, io::MarchStarted{ _unitId, unitPoint.x, unitPoint.y, _targetX, _targetY });
    return ActionResult::success;
}


MoveAction::MoveAction(const std::shared_ptr<EventLog> &eventLog
            , const std::shared_ptr<Map> &map
            , const uint32_t unitId
            , const uint32_t targetX
            , const uint32_t targetY) 
    : SingleAction(eventLog, map)
    , _unitId{unitId}
    , _targetX{std::move(targetX)}
    , _targetY{std::move(targetY)}
{
}

ActionResult MoveAction::exec(const int32_t tick)
{
    auto unit = _map->getUnit(_unitId);
    if (!unit)
    {
        return ActionResult::fail;
    }

    std::tuple<uint32_t, uint32_t, uint32_t> t;
    if (unit->findAndAtack(_map, t))
    {
        uint32_t tId {0};
        uint32_t dam {0};
        uint32_t tHp {0};
        std::tie(tId, dam, tHp) = t;
        _eventLog->log(tick, io::UnitAttacked{_unitId, tId, dam, tHp});
        return ActionResult::skip;
    }

    if (!_map->move(_unitId, _targetX, _targetY))
    {
        return ActionResult::fail;
    }
    _eventLog->log(tick, io::UnitMoved{ _unitId, _targetX, _targetY});
    return ActionResult::success;
}


MarchEndAction::MarchEndAction(const std::shared_ptr<EventLog> &eventLog
                , const std::shared_ptr<Map> &map
                , const uint32_t unitId) 
    : SingleAction(eventLog, map)
    , _unitId{unitId}
{
}

ActionResult MarchEndAction::exec(const int32_t tick)
{
    bool ok = false;
    auto unitPoint = _map->getPoint(_unitId, ok);
    if (!ok)
    {
        return ActionResult::fail;
    }
    _eventLog->log(tick, io::MarchEnded{ _unitId, unitPoint.x, unitPoint.y });
    return ActionResult::success;
}

} // namespace sw
