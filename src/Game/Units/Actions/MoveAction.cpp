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

bool MarchStartAction::exec(const int32_t tick) 
{
    bool ok = false;
    auto unitPoint = _map->getPoint(_unitId, ok);
    if (!ok)
    {
        return false;
    }
    _eventLog->log(tick, io::MarchStarted{ _unitId, unitPoint.x, unitPoint.y, _targetX, _targetY });
    return true;
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

bool MoveAction::exec(const int32_t tick)
{
    auto unit = _map->getUnit(_unitId);
    if (!unit)
    {
        return false;
    }

    uint32_t tId {0};
    uint32_t d {0};
    uint32_t tHp {0};
    auto cb = [&tId, &d, &tHp](const uint32_t targetId, const uint32_t damage, const uint32_t targetHp)
    {
        tId = targetId;
        d = damage;
        tHp = targetHp;
    };
    if (unit->findAndAtack(_map, std::move(cb)))
    {
        _eventLog->log(tick, io::UnitAttacked{_unitId, tId, d, tHp});
        return true;
    }

    if (!_map->move(_unitId, _targetX, _targetY))
    {
        return false;
    }
    _eventLog->log(tick, io::UnitMoved{ _unitId, _targetX, _targetY});
    return true;
}


MarchEndAction::MarchEndAction(const std::shared_ptr<EventLog> &eventLog
                , const std::shared_ptr<Map> &map
                , const uint32_t unitId) 
    : SingleAction(eventLog, map)
    , _unitId{unitId}
{
}

bool MarchEndAction::exec(const int32_t tick)
{
    bool ok = false;
    auto unitPoint = _map->getPoint(_unitId, ok);
    if (!ok)
    {
        return false;
    }
    _eventLog->log(tick, io::MarchEnded{ _unitId, unitPoint.x, unitPoint.y });
    return true;
}

} // namespace sw
