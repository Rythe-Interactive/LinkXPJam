#pragma once
#include <core/core.hpp>
#include <audio/audio.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include "gui_test.hpp"

using namespace legion;

struct player_move : public app::input_axis<player_move> {};
struct player_strive : public app::input_axis<player_strive> {};
struct player_fly : public app::input_axis<player_fly> {};
struct player_look_x : public app::input_axis<player_look_x> {};
struct player_look_y : public app::input_axis<player_look_y> {};
struct player_speedup : public app::input_axis<player_speedup> {};

struct escape_cursor_action : public app::input_action<escape_cursor_action> {};

class SimpleCameraController final : public System<SimpleCameraController>
{
public:
    ecs::entity camera;

    bool escaped = true;
    bool playing = false;
    float movementspeed = 2.f;

    void setup();

    void onGetCamera(time::span);

    void setupCameraEntity();

    void onSpeedUp(player_speedup& action);

    void onEscapeCursor(escape_cursor_action& action);

    void onPlayerMove(player_move& action);

    void onPlayerStrive(player_strive& action);

    void onPlayerFly(player_fly& action);

    void onPlayerLookX(player_look_x& action);

    void onPlayerLookY(player_look_y& action);

};
