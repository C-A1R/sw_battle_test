#pragma once

#include <cstdint>

#include "IUnitAction.hpp"

namespace sw
{

class MarchStartAction : public SingleAction
{
    uint32_t _unitId {0};
    uint32_t _targetX {0};
    uint32_t _targetY {0};
public:
    MarchStartAction(const std::shared_ptr<EventLog> &eventLog
                        , const std::shared_ptr<Map> &map
                        , const uint32_t unitId
                        , const uint32_t targetX
                        , const uint32_t targetY);
    ActionResult exec(const int32_t tick) override;
};

class MoveAction : public SingleAction
{
    uint32_t _unitId {0};
    uint32_t _targetX {0};
    uint32_t _targetY {0};
public:
    MoveAction(const std::shared_ptr<EventLog> &eventLog
                , const std::shared_ptr<Map> &map
                , const uint32_t unitId
                , const uint32_t targetX
                , const uint32_t targetY);
    ActionResult exec(const int32_t tick) override;
};

class MarchEndAction : public SingleAction
{
    uint32_t _unitId {0};
public:
    MarchEndAction(const std::shared_ptr<EventLog> &eventLog
                    , const std::shared_ptr<Map> &map
                    , const uint32_t unitId);

    ActionResult exec(const int32_t tick) override;
};

} // namespace sw
