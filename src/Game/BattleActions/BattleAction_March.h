#pragma once

#include "IBattleAction.h"

#include "../BattleCore/Core.hpp"
#include "../Units/UnitActions/MoveAction.hpp"

namespace sw
{

class BattleAction_March : public IBattleAction
{
    io::March               _cmd;
    std::shared_ptr<Core>   _core;

public:
    BattleAction_March(io::March &&cmd, const std::shared_ptr<Core> &core)
        : _cmd{std::move(cmd)}
        , _core{core}
    {
    }

protected:
    void exec(const uint32_t tick) override
    {
        if (!_core->isValid())
        {
            return;
        }

        std::vector<std::shared_ptr<IUnitAction>> actions;
        splitMarch(actions);
        for (int i = 0; i < actions.size(); ++i)
        {
            _core->addUnitAction(_cmd.unitId, actions[i]);
        }
    }

private:
    /// @brief separate march to several moves
    void splitMarch(std::vector<std::shared_ptr<IUnitAction>> &actions)
    {
        bool ok = true;
        const Point currentPoint = _core->getUnitPoint(_cmd.unitId, ok);
        if (!ok || (currentPoint.x == _cmd.targetX && currentPoint.y == _cmd.targetY))
        {
            return;
        }

        /// the longest march length
        uint32_t marchLen = std::max(currentPoint.x, _cmd.targetX) - std::min(currentPoint.x, _cmd.targetX)
                        + std::max(currentPoint.y, _cmd.targetY) - std::min(currentPoint.y, _cmd.targetY);
        actions.reserve(marchLen + 2);
        //start march
        actions.emplace_back(std::make_shared<MarchStartAction>(_core, _cmd.unitId, _cmd.targetX, _cmd.targetY));
        const Point targetPoint{_cmd.targetX, _cmd.targetY};
        Point tmpPoint = currentPoint;
        //moving
        while (tmpPoint != targetPoint)
        {
            doStep(targetPoint, tmpPoint);
            actions.emplace_back(std::make_shared<MoveAction>(_core, _cmd.unitId, tmpPoint.x, tmpPoint.y));
        }
        //end march
        actions.emplace_back(std::make_shared<MarchEndAction>(_core, _cmd.unitId));
    }

    void doStep(const Point &targetPoint, Point &currentPoint)
    {
        if (currentPoint.x < _cmd.targetX)
        {
            ++currentPoint.x;
        }
        else if (currentPoint.x > _cmd.targetX)
        {
            --currentPoint.x;
        }
        
        if (currentPoint.y < _cmd.targetY)
        {
            ++currentPoint.y;
        }
        else if (currentPoint.y > _cmd.targetY)
        {
            --currentPoint.y;
        }
    }
};

} // namespace sw
