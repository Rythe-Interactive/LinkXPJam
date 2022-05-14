#pragma once
#include "../engine_include.hpp"

#include "../renderstages/mousehover.hpp"
#include "gui_test.hpp"

#include "../data/collision_data.hpp"

struct [[reflectable]] example_comp
{
    int value = 1;
};

struct tonemap_action : public lgn::app::input_action<tonemap_action> {};
struct switch_skybox_action : public lgn::app::input_action<switch_skybox_action> {};
struct auto_exposure_action : public lgn::app::input_action<auto_exposure_action> {};
struct reload_shaders_action : public lgn::app::input_action<reload_shaders_action> {};


struct exit_action : public lgn::app::input_action<exit_action> {};
struct restart_action : public lgn::app::input_action<restart_action> {};

struct fullscreen_action : public lgn::app::input_action<fullscreen_action> {};
struct vsync_action : public lgn::app::input_action<vsync_action> {};

class GameSystem final : public legion::System<GameSystem>
{
public:
    lgn::ecs::entity player;
    
    float score = 0;
    float linearMovement = 300.f;
    float radialMovement = 10.f;

    bool escaped = false;

    void setup();
    void fixedUpdate(lgn::time::span deltaTime);
    void onGUI(app::window& context, gfx::camera& cam, const gfx::camera::camera_input& camInput, time::span deltaTime);

    void spawnEnemy();

    void shoot(ecs::entity ship);


    void onCollision(collision& event);
    void initInput();

    void onAutoExposureSwitch(auto_exposure_action& event);
    void onTonemapSwitch(tonemap_action& event);
    void onExit(exit_action& action);
    void onFullscreen(fullscreen_action& action);
    void onVSYNCSwap(vsync_action& action);
};
