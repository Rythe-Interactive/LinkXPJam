#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../components/components.hpp"

struct player_horizontal : public lgn::app::input_axis<player_horizontal> {};
struct player_vertical : public lgn::app::input_axis<player_vertical> {};
struct player_shoot : public lgn::app::input_action<player_shoot> {};

using namespace lgn;
class PlayerSystem final : public legion::System<PlayerSystem>
{
    ecs::filter<position, rotation, scale, player_comp> players;
    ecs::filter<position, rotation, scale, rendering::camera > cameras;
    ecs::entity player;
    ecs::entity camera;

    math::vec2 movement;
    float speed = 2.f;
    bool shooting = false;

public:
    void setup();
    void fixedUpdate(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("PlayerSystem shutdown");
    }

    void onShoot(player_shoot& action);
    void shoot();
    void horizontal_move(player_horizontal& axis);
    void vertical_move(player_vertical& axis);
};
