#include "MoveAction.hpp"

#include "../../BattleCore/Core.hpp"
#include "../IUnit.hpp"

#include "../../../IO/System/EventLog.hpp"
#include "../../../IO/Events/MarchStarted.hpp"
#include "../../../IO/Events/UnitMoved.hpp"
#include "../../../IO/Events/MarchEnded.hpp"

namespace sw
{

MarchStartAction::MarchStartAction(const std::shared_ptr<Core> &core
                                 , const uint32_t unitId
                                 , const uint32_t targetX
                                 , const uint32_t targetY)
    : SingleAction(core)
    , _unitId{unitId}
    , _targetX{std::move(targetX)}
    , _targetY{std::move(targetY)}
{
}

ActionResult MarchStartAction::exec(const int32_t tick) 
{
    bool ok = false;
    auto unitPoint = _core->getUnitPoint(_unitId, ok);
    if (!ok)
    {
        return ActionResult::fail;
    }
    EventLog::instance().log(tick, io::MarchStarted{ _unitId, unitPoint.x, unitPoint.y, _targetX, _targetY });
    return ActionResult::success;
}


MoveAction::MoveAction(const std::shared_ptr<Core> &core
            , const uint32_t unitId
            , const uint32_t targetX
            , const uint32_t targetY) 
    : SingleAction(core)
    , _unitId{unitId}
    , _targetX{std::move(targetX)}
    , _targetY{std::move(targetY)}
{
}

ActionResult MoveAction::exec(const int32_t tick)
{
    auto unit = _core->getUnit(_unitId);
    if (!unit)
    {
        return ActionResult::fail;
    }

    if (unit->findAndAtack(_core, tick))
    {
        return ActionResult::skip;
    }

    if (!_core->move(_unitId, _targetX, _targetY))
    {
        return ActionResult::fail;
    }
    EventLog::instance().log(tick, io::UnitMoved{ _unitId, _targetX, _targetY});
    return ActionResult::success;
}


MarchEndAction::MarchEndAction(const std::shared_ptr<Core> &core, const uint32_t unitId) 
    : SingleAction(core)
    , _unitId{unitId}
{
}

ActionResult MarchEndAction::exec(const int32_t tick)
{
    bool ok = false;
    auto unitPoint = _core->getUnitPoint(_unitId, ok);
    if (!ok)
    {
        return ActionResult::fail;
    }
    EventLog::instance().log(tick, io::MarchEnded{ _unitId, unitPoint.x, unitPoint.y });
    return ActionResult::success;
}

} // namespace sw
