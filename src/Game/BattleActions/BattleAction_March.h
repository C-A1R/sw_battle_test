#pragma once

#include "IBattleAction.h"

#include "../Map.hpp"
#include "../Units/Actions/MoveAction.hpp"

namespace sw
{

class BattleAction_March : public IBattleAction
{
    io::March               _cmd;
    std::shared_ptr<Map>    _map;
public:
    BattleAction_March(io::March &&cmd, std::shared_ptr<Map> map)
        : _cmd{std::move(cmd)}
        , _map{map}
    {
    }
protected:
    void exec(const uint32_t tick) override
    {
        if (!_map || !_map->isValid())
        {
            return;
        }

        auto unit = _map->_units->find(_cmd.unitId);
        if (!unit)
        {
            return;
        }

        std::vector<std::shared_ptr<IUnitAction>> actions;
        splitMarch(actions);
        for (int i = 0; i < actions.size(); ++i)
        {
            unit->addAction(actions[i]);
        }
    }

    /// @brief separate march to several moves
    void splitMarch(std::vector<std::shared_ptr<IUnitAction>> &actions)
    {
        bool ok = true;
        const Point currentPoint = _map->getPoint(_cmd.unitId, ok);
        if (!ok || (currentPoint.x == _cmd.targetX && currentPoint.y == _cmd.targetY))
        {
            return;
        }

        /// the longest march length
        uint32_t marchLen = std::max(currentPoint.x, _cmd.targetX) - std::min(currentPoint.x, _cmd.targetX)
                        + std::max(currentPoint.y, _cmd.targetY) - std::min(currentPoint.y, _cmd.targetY);
        actions.reserve(marchLen + 2);
        //start march
        actions.emplace_back(std::make_shared<MarchStartAction>(_map, _cmd.unitId, _cmd.targetX, _cmd.targetY));
        Point tmpPoint = currentPoint;
        //move
        auto _doStep = [&tmpPoint, this]()
        {
            if (tmpPoint.x < _cmd.targetX)
            {
                ++tmpPoint.x;
            }
            else if (tmpPoint.x > _cmd.targetX)
            {
                --tmpPoint.x;
            }
            
            if (tmpPoint.y < _cmd.targetY)
            {
                ++tmpPoint.y;
            }
            else if (tmpPoint.y > _cmd.targetY)
            {
                --tmpPoint.y;
            }
        };
        while (!(tmpPoint.x == _cmd.targetX && tmpPoint.y == _cmd.targetY))
        {
            _doStep();
            actions.emplace_back(std::make_shared<MoveAction>(_map, _cmd.unitId, tmpPoint.x, tmpPoint.y));
        }
        //end march
        actions.emplace_back(std::make_shared<MarchEndAction>(_map, _cmd.unitId));
    }
};

} // namespace sw
