#include "../systems/simplecameracontroller.hpp"


void SimpleCameraController::setup()
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

void SimpleCameraController::onGetCamera(time::span)
{
    static ecs::filter<gfx::camera> query{};

    if (query.size())
    {
        camera = query[0];
    }
}

void SimpleCameraController::setupCameraEntity()
{
    camera = createEntity("Camera");
    camera.add_component<transform>(position(0.f, 20.f, 0.f), rotation::lookat(position(0.f, 20.f, 0.f), math::vec3(0.f, 0.f, 0.0001f), math::vec3::up), scale());
    camera.add_component<audio::audio_listener>();

    rendering::camera cam;
    cam.set_projection(60.f, 0.001f, 1000.f);
    camera.add_component<gfx::camera>(cam);
}

#pragma region input stuff
void SimpleCameraController::onSpeedUp(player_speedup& action)
{
    if (playing)
        return;

    if (GuiTestSystem::isEditingText)
        return;

    movementspeed = 5.f + action.value;
}

void SimpleCameraController::onEscapeCursor(escape_cursor_action& action)
{
    if (playing)
        return;

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

void SimpleCameraController::onPlayerMove(player_move& action)
{
    if (playing)
        return;

    if (GuiTestSystem::isEditingText)
        return;

    if (escaped)
        return;

    position& pos = camera.get_component<position>();
    rotation& rot = camera.get_component<rotation>();
    math::vec3 move = math::toMat3(rot) * math::vec3(0.f, 0.f, 1.f);
    pos += math::normalize(move * math::vec3(1, 0, 1)) * action.value * action.input_delta * movementspeed;
}

void SimpleCameraController::onPlayerStrive(player_strive& action)
{
    if (playing)
        return;

    if (GuiTestSystem::isEditingText)
        return;

    if (escaped)
        return;

    position& pos = camera.get_component<position>();
    rotation& rot = camera.get_component<rotation>();
    math::vec3 move = math::toMat3(rot) * math::vec3(1.f, 0.f, 0.f);
    pos += math::normalize(move * math::vec3(1, 0, 1)) * action.value * action.input_delta * movementspeed;
}

void SimpleCameraController::onPlayerFly(player_fly& action)
{
    if (playing)
        return;

    if (GuiTestSystem::isEditingText)
        return;

    if (escaped)
        return;

    position& pos = camera.get_component<position>();
    pos += (math::vec3(0.f, action.value * action.input_delta * movementspeed, 0.f));
}

void SimpleCameraController::onPlayerLookX(player_look_x& action)
{
    if (playing)
        return;

    if (GuiTestSystem::isEditingText)
        return;

    if (escaped)
        return;

    rotation& rot = camera.get_component<rotation>();
    rot = math::angleAxis(action.value * action.input_delta * 500.f, math::vec3::up) * rot;
}

void SimpleCameraController::onPlayerLookY(player_look_y& action)
{
    if (playing)
        return;

    if (GuiTestSystem::isEditingText)
        return;

    if (escaped)
        return;

    rotation& rot = camera.get_component<rotation>();
    math::mat3 rotMat = math::toMat3(rot);
    math::vec3 right = rotMat * math::vec3::right;
    math::vec3 fwd = math::normalize(math::cross(right, math::vec3::up));
    math::vec3 up = rotMat * math::vec3::up;
    float angle = math::orientedAngle(fwd, up, right);

    angle += action.value * action.input_delta * 500.f;

    if (angle > -0.001f)
        angle = -0.001f;
    if (angle < -(math::pi<float>() - 0.001f))
        angle = -(math::pi<float>() - 0.001f);

    up = math::mat3(math::axisAngleMatrix(right, angle)) * fwd;
    fwd = math::cross(right, up);
    rot = (rotation)math::conjugate(math::toQuat(math::lookAt(math::vec3::zero, fwd, up)));
}
#pragma endregion


