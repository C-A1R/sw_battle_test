#pragma once

#include <cstdint>
#include <queue>
#include <memory>

#include "IUnitAction.hpp"

namespace sw
{

/// @brief  Unit interface
class IUnit
{
public:
    IUnit() = default;
    virtual ~IUnit() = default;

    virtual uint32_t id() const = 0;
    virtual void setMap(const std::shared_ptr<IMap> &map) = 0;

    virtual bool spawn(uint32_t x, uint32_t y) = 0;
    virtual void march(uint32_t targetX, uint32_t targetY, std::function<void(const int32_t tick)> &&callback) = 0;

    virtual bool execAction(const int32_t tick) = 0;
};

/// @brief  Unit base class
class Unit : public IUnit
{
    const uint32_t          _id {0};
    uint32_t                _hp {0};
    std::shared_ptr<IMap>   _map;
    std::queue<std::unique_ptr<IUnitAction>> _actions;

public:
    Unit(const uint32_t id, const uint32_t hp)
        : _id{id}
        , _hp{hp}
    {
    }
    ~Unit() override = default;

protected:
    uint32_t id() const override { return _id; }
    void setMap(const std::shared_ptr<IMap> &map) override { _map = map; }

    bool spawn(uint32_t x, uint32_t y) override { return _map->spawn(_id, x, y); }
    void march(uint32_t targetX, uint32_t targetY, std::function<void(const int32_t)> &&callback) override
    {
        /// @todo
        _actions.emplace(std::make_unique<MarchAction>(targetX, targetY, std::move(callback)));
    }

    bool execAction(const int32_t tick) override
    {
        if (_actions.empty())
        {
            return false;
        }

        auto &act = _actions.front();
        act->exec(tick);
        _actions.pop();
        return true;
    }
};

} // namespace sw
