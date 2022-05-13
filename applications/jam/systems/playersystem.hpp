#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../components/components.hpp"

using namespace lgn;
class PlayerSystem final : public legion::System<PlayerSystem>
{
    ecs::filter<position, rotation, scale, rigidbody> players;
    ecs::filter<position, rotation, scale, rendering::camera, rigidbody> cameras;

    ecs::entity player;
    ecs::entity camera;
public:
    void setup();
    void fixedUpdate(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("PlayerSystem shutdown");
    }
};
