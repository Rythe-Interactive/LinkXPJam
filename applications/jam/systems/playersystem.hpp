#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../components/components.hpp"

struct player_horizontal : public lgn::app::input_axis<player_horizontal> {};
struct player_vertical : public lgn::app::input_axis<player_vertical> {};
struct player_shoot : public lgn::app::input_action<player_shoot> {};

enum PlayerAnimState
{
    IDLE,
    WALK,
    SHOOT,
    MELEE
};

using namespace lgn;
class PlayerSystem final : public legion::System<PlayerSystem>
{
    ecs::filter<player_comp> players;
    ecs::filter<position, rotation, scale, rendering::camera > cameras;
    ecs::entity player;
    ecs::entity camera;

    PlayerAnimState animState = PlayerAnimState::IDLE;

    std::unordered_map<id_type, key_frame_list> animations;

    math::vec2 movement;
    float speed = 10.f;
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

    void resolveAnimationState();
    void stopCurrentAnim();
};
