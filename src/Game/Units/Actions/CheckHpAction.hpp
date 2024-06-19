#pragma once

#include <cstdint>
#include <functional>

#include "IUnitAction.hpp"
#include "../../Maps/Map.hpp"
#include "../IUnit.hpp"

namespace sw
{

class CheckHpAction : public SingleAction
{
    const int32_t _unitId {0};
    std::function<void(const int32_t, const int32_t)>  _callback;
public:
    CheckHpAction(const int32_t &unitId, std::function<void(const int32_t, const int32_t)> &&callback) 
        : _unitId{unitId}
        , _callback{std::move(callback)}
    {
    }
    bool exec(const int32_t tick) override
    {
        _callback(tick, _unitId);
        return true;
    }
};


} // namespace sw
