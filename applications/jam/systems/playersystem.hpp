#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../components/components.hpp"

struct player_horizontal : public lgn::app::input_axis<player_horizontal> {};
struct player_vertical : public lgn::app::input_axis<player_vertical> {};

using namespace lgn;
class PlayerSystem final : public legion::System<PlayerSystem>
{
    ecs::filter<position, rotation, scale> players;

    ecs::entity player;
public:
    void setup();
    void fixedUpdate(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("PlayerSystem shutdown");
    }

    void horizontal_move(player_horizontal& axis);
    void vertical_move(player_vertical& axis);
};
