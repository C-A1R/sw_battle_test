#pragma once

#include <cstdint>

#include "../IUnit.hpp"

namespace sw
{

class IUnitAction
{
public:
    IUnitAction() = default;
    virtual ~IUnitAction() = default;
    virtual bool exec(const int32_t) = 0;
};

} // namespace sw
