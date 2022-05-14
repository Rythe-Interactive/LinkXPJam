#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../components/components.hpp"

using namespace lgn;
class KillableSystem final : public legion::System<KillableSystem>
{
    ecs::filter<killable> killables;

public:
    void setup();
    void fixedUpdate(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("KillableSystem shutdown");
    }

};
