#include "../systems/playersystem.hpp"
#include "../debug_utils.hpp"
#include <rendering/pipeline/default/stages/framebufferresizestage.hpp>

void PlayerSystem::setup()
{
    using namespace lgn;
    log::debug("PlayerSystem setup");

    auto player = createEntity();
    position& pos = player.add_component<position>();
    player.add_component<rotation>();
    scale& scal = player.add_component<scale>();
    scal = scale(1.5f);
    player.add_component<rigidbody>();
    player_comp& playerComp = player.add_component<player_comp>();
    killable& k = player.add_component<killable>();
    k.health = playerComp.initHealth;
    animations.emplace(nameHash("player_idle"), key_frame_list{
                { gfx::ModelCache::create_model("player_idle0", fs::view("assets://models/player/idle/player_idle0.glb")), 0.05f },
        });
    animations.emplace(nameHash("player_melee"), key_frame_list{
            { gfx::ModelCache::create_model("player_melee0", fs::view("assets://models/player/melee/player_melee0.glb")), 0.05f },
            { gfx::ModelCache::create_model("player_melee1", fs::view("assets://models/player/melee/player_melee1.glb")), 0.05f },
            { gfx::ModelCache::create_model("player_melee2", fs::view("assets://models/player/melee/player_melee2.glb")), 0.05f },
            { gfx::ModelCache::create_model("player_melee3", fs::view("assets://models/player/melee/player_melee3.glb")), 0.05f },
            { gfx::ModelCache::create_model("player_melee4", fs::view("assets://models/player/melee/player_melee4.glb")), 0.05f },
            { gfx::ModelCache::create_model("player_melee5", fs::view("assets://models/player/melee/player_melee5.glb")), 0.05f },
            { gfx::ModelCache::create_model("player_melee6", fs::view("assets://models/player/melee/player_melee6.glb")), 0.05f }
        });
    animations.emplace(nameHash("player_shoot"), key_frame_list{
        { gfx::ModelCache::create_model("player_shoot0", fs::view("assets://models/player/shoot/player_shoot0.glb")), 0.05f },
        { gfx::ModelCache::create_model("player_shoot1", fs::view("assets://models/player/shoot/player_shoot1.glb")), 0.05f },
        { gfx::ModelCache::create_model("player_shoot2", fs::view("assets://models/player/shoot/player_shoot2.glb")), 0.05f },
        { gfx::ModelCache::create_model("player_shoot3", fs::view("assets://models/player/shoot/player_shoot3.glb")), 0.05f },
        { gfx::ModelCache::create_model("player_shoot4", fs::view("assets://models/player/shoot/player_shoot4.glb")), 0.05f }
        });
    animated_mesh_renderer& animationAvatar = player.add_component(animated_mesh_renderer(gfx::MaterialCache::get_material("default"), animations[nameHash("player_idle")]));
    animationAvatar.playing = false;

    if (!cameras.empty())
    {
        camera = cameras[0];
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
    for (auto ent : players)
    {
        player = ent;
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

        position& pos = ent.get_component<position>();
        rigidbody& rb = ent.get_component<rigidbody>();
        rb.velocity = math::vec3(movement.x, 0.f, movement.y);

        rotation& rot = ent.get_component<rotation>();
        rot = rotation::lookat(pos, lookAtPos);

        if (shooting)
            shoot();

        resolveAnimationState();
    }
}

void PlayerSystem::resolveAnimationState()
{
    animated_mesh_renderer& anim = player.get_component<animated_mesh_renderer>();

    if (animState == PlayerAnimState::IDLE)
    {
        anim.keyFrames = animations[nameHash("player_idle")];
        anim.playing = true;
        anim.looping = false;
        return;
    }

    if (animState == PlayerAnimState::WALK)
    {
        anim.keyFrames = animations[nameHash("player_walk")];
        anim.playing = true;
        anim.looping = true;
        return;
    }

    if (animState == PlayerAnimState::SHOOT)
    {
        anim.keyFrames = animations[nameHash("player_shoot")];
        anim.playing = true;
        anim.looping = false;
        return;
    }

    if (animState == PlayerAnimState::MELEE)
    {
        anim.keyFrames = animations[nameHash("player_melee")];
        anim.playing = true;
        anim.looping = false;
        return;
    }
}

void PlayerSystem::stopCurrentAnim()
{
    animated_mesh_renderer& anim = player.get_component<animated_mesh_renderer>();
    anim.playing = false;
    anim.looping = false;
    animState = PlayerAnimState::IDLE;
}

void PlayerSystem::onShoot(player_shoot& action)
{
    shooting = action.pressed();
}

void PlayerSystem::shoot()
{
    animState = PlayerAnimState::SHOOT;
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
    col.layer = 1;
    col.ignoreMask = 1;
    col.add_shape<SphereCollider>();
}

void PlayerSystem::horizontal_move(player_horizontal& axis)
{
    if (axis.value > 0.f || axis.value < 0.f)
    {
        movement.x = axis.value * speed;
        animState = PlayerAnimState::IDLE;
    }
}

void PlayerSystem::vertical_move(player_vertical& axis)
{
    if (axis.value > 0.f || axis.value < 0.f)
    {
        movement.y = axis.value * speed;
        animState = PlayerAnimState::IDLE;
    }
}

