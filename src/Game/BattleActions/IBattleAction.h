#pragma once

namespace sw
{

class IBattleAction
{
public:
    IBattleAction() = default;
    virtual ~IBattleAction() = default;

    virtual void exec(const uint32_t tick) = 0;
};

} // namespace sw
