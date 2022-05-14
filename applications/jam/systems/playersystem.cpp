#include "../systems/playersystem.hpp"
#include "../debug_utils.hpp"
#include <rendering/pipeline/default/stages/framebufferresizestage.hpp>

void PlayerSystem::setup()
{
    using namespace lgn;
    log::debug("PlayerSystem setup");

    for (ecs::entity ent : players)
    {
        player = ent;
    }

    for (ecs::entity ent : cameras)
    {
        camera = ent;
    }

    app::InputSystem::createBinding<player_horizontal>(app::inputmap::method::A, -1);
    app::InputSystem::createBinding<player_horizontal>(app::inputmap::method::D, 1);
    app::InputSystem::createBinding<player_vertical>(app::inputmap::method::W, 1);
    app::InputSystem::createBinding<player_vertical>(app::inputmap::method::S, -1);
    app::InputSystem::createBinding<player_shoot>(app::inputmap::method::MOUSE_LEFT);

    bindToEvent<player_shoot, &PlayerSystem::onShoot>();
    bindToEvent<player_horizontal, &PlayerSystem::horizontal_move>();
    bindToEvent<player_vertical, &PlayerSystem::vertical_move>();

    createProcess<&PlayerSystem::fixedUpdate>("Update", 0.02f);
}


void PlayerSystem::fixedUpdate(lgn::time::span deltaTime)
{
    if (player)
    {
        app::window& context = ecs::world.get_component<app::window>();
        float renderScale = rendering::FramebufferResizeStage::getRenderScale();
        math::ivec2 framebufferSize = context.framebufferSize();
        framebufferSize.x *= renderScale;
        framebufferSize.y *= renderScale;

        auto mousePos = app::InputSystem::getMousePosition();
        mousePos.y = 1.0 - mousePos.y;
        mousePos -= math::dvec2(.5, .5);
        mousePos *= 2.f;

        transform trans = camera.get_component<transform>();
        math::mat4 view = trans.from_world_matrix();
        math::mat4 projection = camera.get_component<rendering::camera>()->get_projection(((float)framebufferSize.x) / framebufferSize.y);
        math::mat4 screenToWorld = math::inverse(projection * view);
        auto worldMousePos = screenToWorld * math::vec4(mousePos.x, mousePos.y, 0.0f, 1.f);
        position lookAtPos = math::vec3(worldMousePos.x * 20.f, 0.f, worldMousePos.z * 20.f);

        position& pos = player.get_component<position>();
        pos += math::vec3(movement.x, 0.f, movement.y);

        rotation& rot = player.get_component<rotation>();
        rot = rotation::lookat(pos, lookAtPos);

        if (shooting)
            shoot();
    }
}

void PlayerSystem::onShoot(player_shoot& action)
{
    shooting = action.pressed();
}

void PlayerSystem::shoot()
{
    auto bullet = createEntity("Bullet");
    bullet.add_component<bullet_comp>();
    position& pos = bullet.add_component<position>();
    pos = player.get_component<position>();
    bullet.add_component<rotation>();
    scale& scal = bullet.add_component<scale>();
    scal = scale(.1f);
    bullet.add_component(gfx::mesh_renderer{ gfx::MaterialCache::get_material("default"), rendering::ModelCache::get_handle("Sphere") });
    rigidbody& rb = bullet.add_component<rigidbody>();
    rb.addForce(player.get_component<rotation>()->forward() * 500.f);
    rb.linearDrag = .1f;

    collider& col = bullet.add_component<collider>();
    col.layer = 4;
    col.ignoreMask = 4;
    col.add_shape<SphereCollider>();
}

void PlayerSystem::horizontal_move(player_horizontal& axis)
{
    movement.x = axis.value * axis.input_delta * speed;
}

void PlayerSystem::vertical_move(player_vertical& axis)
{
    movement.y = axis.value * axis.input_delta * speed;
}

