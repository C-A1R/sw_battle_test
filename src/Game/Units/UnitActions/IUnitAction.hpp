#pragma once

#include "../../../IO/System/PrintDebug.hpp"

#include <cstdint>
#include <memory>

namespace sw
{

class Core;

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
    std::shared_ptr<Core> _core;
public:
    SingleAction(){}
    SingleAction(const std::shared_ptr<Core> &core)
        : _core{core}
    {
    }
};

} // namespace sw
