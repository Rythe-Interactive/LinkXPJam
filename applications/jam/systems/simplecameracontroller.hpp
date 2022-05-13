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
    float movementspeed = 5.f;

    void setup()
    {
#pragma region Input binding
        app::InputSystem::createBinding<player_move>(app::inputmap::method::W, 1.f);
        app::InputSystem::createBinding<player_move>(app::inputmap::method::S, -1.f);
        app::InputSystem::createBinding<player_strive>(app::inputmap::method::D, 1.f);
        app::InputSystem::createBinding<player_strive>(app::inputmap::method::A, -1.f);
        app::InputSystem::createBinding<player_fly>(app::inputmap::method::SPACE, 1.f);
        app::InputSystem::createBinding<player_fly>(app::inputmap::method::LEFT_SHIFT, -1.f);
        app::InputSystem::createBinding<player_look_x>(app::inputmap::method::MOUSE_X, 0.f);
        app::InputSystem::createBinding<player_look_y>(app::inputmap::method::MOUSE_Y, 0.f);
        app::InputSystem::createBinding<player_speedup>(app::inputmap::method::LEFT_CONTROL, 3.f);
        app::InputSystem::createBinding<player_speedup>(app::inputmap::method::LEFT_ALT, -3.f);

        app::InputSystem::createBinding<escape_cursor_action>(app::inputmap::method::MOUSE_RIGHT);

        bindToEvent<player_move, &SimpleCameraController::onPlayerMove>();
        bindToEvent<player_strive, &SimpleCameraController::onPlayerStrive>();
        bindToEvent<player_fly, &SimpleCameraController::onPlayerFly>();
        bindToEvent<player_look_x, &SimpleCameraController::onPlayerLookX>();
        bindToEvent<player_look_y, &SimpleCameraController::onPlayerLookY>();
        bindToEvent<player_speedup, &SimpleCameraController::onSpeedUp>();
        bindToEvent<escape_cursor_action, &SimpleCameraController::onEscapeCursor>();

#pragma endregion

        app::window& window = ecs::world.get_component<app::window>();
        window.enableCursor(true);
        window.show();

        {
            app::context_guard guard(window);
            setupCameraEntity();
        }

        createProcess<&SimpleCameraController::onGetCamera>("Update", 0.5f);
    }

    void onGetCamera(time::span)
    {
        static ecs::filter<gfx::camera> query{};

        if (query.size())
        {
            camera = query[0];
        }
    }

    void setupCameraEntity()
    {
        camera = createEntity("Camera");
        camera.add_component<transform>(position(0.f, 3.f, -30.f), rotation::lookat(math::float3::zero, math::float3::forward), scale());
        camera.add_component<audio::audio_listener>();

        rendering::camera cam;
        cam.set_projection(60.f, 0.001f, 1000.f);
        camera.add_component<gfx::camera>(cam);
    }

#pragma region input stuff
    void onSpeedUp(player_speedup& action)
    {
        if (GuiTestSystem::isEditingText)
            return;

        movementspeed = 5.f + action.value;
    }

    void onEscapeCursor(escape_cursor_action& action)
    {
        if (GuiTestSystem::isEditingText)
            return;

        if (action.released() && !escaped)
        {
            app::window& window = ecs::world.get_component<app::window>();
            escaped = true;
            window.enableCursor(true);
        }
        else if (action.pressed() && escaped)
        {
            app::window& window = ecs::world.get_component<app::window>();
            escaped = false;
            window.enableCursor(false);
        }
        GuiTestSystem::CaptureKeyboard(!escaped);
    }

    void onPlayerMove(player_move& action)
    {
        if (GuiTestSystem::isEditingText)
            return;

        if (escaped)
            return;

        position& pos = camera.get_component<position>();
        rotation& rot = camera.get_component<rotation>();
        math::float3 move = rot.forward();
        pos += math::normalize(move * math::float3(1, 0, 1)) * action.value * action.input_delta * movementspeed;
    }

    void onPlayerStrive(player_strive& action)
    {
        if (GuiTestSystem::isEditingText)
            return;

        if (escaped)
            return;

        position& pos = camera.get_component<position>();
        rotation& rot = camera.get_component<rotation>();
        math::float3 move = rot.right();
        pos += math::normalize(move * math::float3(1, 0, 1)) * action.value * action.input_delta * movementspeed;
    }

    void onPlayerFly(player_fly& action)
    {
        if (GuiTestSystem::isEditingText)
            return;

        if (escaped)
            return;

        position& pos = camera.get_component<position>();
        pos += (math::float3(0.f, action.value * action.input_delta * movementspeed, 0.f));
    }

    void onPlayerLookX(player_look_x& action)
    {
        if (GuiTestSystem::isEditingText)
            return;

        if (escaped)
            return;

        rotation& rot = camera.get_component<rotation>();
        rot = math::angleAxis(action.value * action.input_delta * 500.f, math::float3::up) * rot;
    }

    void onPlayerLookY(player_look_y& action)
    {
        if (GuiTestSystem::isEditingText)
            return;

        if (escaped)
            return;

        rotation& rot = camera.get_component<rotation>();
        math::float3 right = rot.right();
        math::float3 fwd = math::normalize(math::cross(right, math::float3::up));
        math::float3 up = rot.up();
        float angle = std::acos(math::dot(fwd, up));

        angle += action.value * action.input_delta * 500.f;

        if (angle > -0.001f)
            angle = -0.001f;
        if (angle < -(math::pi<float>() - 0.001f))
            angle = -(math::pi<float>() - 0.001f);

        up = fwd * math::angleAxis(angle, right);
        fwd = math::cross(right, up);
        rot = rotation::lookat(math::float3::zero, fwd, up);
    }
#pragma endregion

};
