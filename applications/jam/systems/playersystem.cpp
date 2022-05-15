#include "../systems/playersystem.hpp"
#include "../debug_utils.hpp"
#include <rendering/pipeline/default/stages/framebufferresizestage.hpp>

void PlayerSystem::setup()
{
    using namespace lgn;
    log::debug("PlayerSystem setup");

    player = createEntity();
    position& pos = player.add_component<position>();
    pos += math::vec3::right * 5.f;
    player.add_component<rotation>();
    scale& scal = player.add_component<scale>();
    scal = scale(1.5f);
    player.add_component<rigidbody>();
    player_comp& playerComp = player.add_component<player_comp>();

    meleeCol = createEntity("meleeCol");
    meleeCol.add_component<position>(pos + position(math::vec3::forward * 0.5f));
    meleeCol.add_component(scale(1.5f));
    meleeCol.add_component<rotation>();
    {
        collider& col = meleeCol.add_component<collider>();
        col.add_shape<SphereCollider>();
        col.layer = 1;
        col.ignoreMask = 1;
    }

    collider& col = player.add_component<collider>();
    col.add_shape<SphereCollider>();
    col.layer = 1;
    col.ignoreMask = 1;

    killable& k = player.add_component<killable>();
    k.health = playerComp.initHealth;
    animations.emplace(nameHash("player_idle"), key_frame_list{
                { gfx::ModelCache::create_model("player_idle0", fs::view("assets://models/player/idle/player_idle0.glb")), 0.05f },
        });
    animations.emplace(nameHash("player_walk"), key_frame_list{
                { gfx::ModelCache::create_model("player_idle0", fs::view("assets://models/player/idle/player_idle0.glb")), 0.05f },
        });
    animations.emplace(nameHash("player_melee"), key_frame_list{
            { gfx::ModelCache::create_model("player_melee0", fs::view("assets://models/player/melee/player_melee0.glb")), 0.05f },
            { gfx::ModelCache::create_model("player_melee1", fs::view("assets://models/player/melee/player_melee1.glb")), 0.025f },
            { gfx::ModelCache::create_model("player_melee2", fs::view("assets://models/player/melee/player_melee2.glb")), 0.025f },
            { gfx::ModelCache::create_model("player_melee3", fs::view("assets://models/player/melee/player_melee3.glb")), 0.025f },
            { gfx::ModelCache::create_model("player_melee4", fs::view("assets://models/player/melee/player_melee4.glb")), 0.025f },
            { gfx::ModelCache::create_model("player_melee5", fs::view("assets://models/player/melee/player_melee5.glb")), 0.025f },
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
    app::InputSystem::createBinding<player_melee>(app::inputmap::method::MOUSE_RIGHT);

    bindToEvent<player_shoot, &PlayerSystem::onShoot>();
    bindToEvent<player_melee, &PlayerSystem::onMelee>();
    bindToEvent<player_horizontal, &PlayerSystem::horizontal_move>();
    bindToEvent<player_vertical, &PlayerSystem::vertical_move>();
    bindToEvent<collision, &PlayerSystem::onCollision>();

    createProcess<&PlayerSystem::fixedUpdate>("Update", 0.02f);
}


void PlayerSystem::fixedUpdate(lgn::time::span deltaTime)
{
    auto pos = *player.get_component<position>();
    auto rot = *player.get_component<rotation>();
    *meleeCol.get_component<position>() = pos + position(rot.forward() * 0.5f);

    switch (animState)
    {
    case player_anim_state::IDLE:
    case player_anim_state::WALK:
        move();
        break;
    case player_anim_state::SHOOT:
        move();
        break;
    case player_anim_state::MELEE:
        break;
    default:
        break;
    }
}

void PlayerSystem::setAnimationState(player_anim_state state)
{
    animated_mesh_renderer& anim = player.get_component<animated_mesh_renderer>();
    animState = state;
    switch (state)
    {
    case player_anim_state::IDLE:
        anim.keyFrames = animations[nameHash("player_idle")];
        anim.onAnimFrameSwitch = nullptr;
        anim.currentFrame = 0ull;
        anim.playing = true;
        anim.looping = false;
        break;
    case player_anim_state::WALK:
        anim.keyFrames = animations[nameHash("player_walk")];
        anim.onAnimFrameSwitch = nullptr;
        anim.currentFrame = 0ull;
        anim.playing = true;
        anim.looping = true;
        break;
    case player_anim_state::SHOOT:
        anim.keyFrames = animations[nameHash("player_shoot")];
        anim.onAnimFrameSwitch = [&](lgn::index_type nextFrame)
        {
            if (nextFrame == 0)
            {
                canShoot = true;
                setAnimationState(player_anim_state::IDLE);
            }
            else if (nextFrame == 1)
            {
                shoot();
            }
        };
        anim.currentFrame = 0ull;
        anim.playing = true;
        anim.looping = false;
        break;
    case player_anim_state::MELEE:
        anim.keyFrames = animations[nameHash("player_melee")];
        anim.onAnimFrameSwitch = [&](lgn::index_type nextFrame)
        {
            if (nextFrame == 0)
            {
                canShoot = true;
                setAnimationState(player_anim_state::IDLE);
                melee = false;
            }
        };
        anim.currentFrame = 0ull;
        anim.playing = true;
        anim.looping = false;
        break;
    default:
        break;
    }
}

void PlayerSystem::stopCurrentAnim()
{
    animated_mesh_renderer& anim = player.get_component<animated_mesh_renderer>();
    anim.playing = false;
    anim.looping = false;
    animState = player_anim_state::IDLE;
}

void PlayerSystem::move()
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
    rigidbody& rb = player.get_component<rigidbody>();

    if (math::close_enough(movement.x, 0.f) && math::close_enough(movement.y, 0.f))
    {
        if (animState == player_anim_state::WALK)
            setAnimationState(player_anim_state::IDLE);
    }
    else if (animState == player_anim_state::IDLE)
        setAnimationState(player_anim_state::WALK);

    rb.velocity = math::vec3(movement.x, 0.f, movement.y);

    rotation& rot = player.get_component<rotation>();
    rot = rotation::lookat(pos, lookAtPos);
}

void PlayerSystem::onCollision(collision& event)
{
    if (melee)
    {
        ecs::entity first = event.first;
        ecs::entity second = event.second;

        if (second == meleeCol && first.has_component<enemy_comp>())
        {
            auto& secondPos = *first.get_component<position>();
            auto playerPos = *player.get_component<position>();
            secondPos += math::normalize(secondPos - playerPos);
            first.get_component<killable>()->health -= 3.f;
        }
        else if (first == meleeCol && second.has_component<enemy_comp>())
        {
            auto& secondPos = *second.get_component<position>();
            auto playerPos = *player.get_component<position>();
            secondPos += math::normalize(secondPos - playerPos);
            second.get_component<killable>()->health -= 3.f;
        }
    }
}

void PlayerSystem::onShoot(player_shoot& action)
{
    if (action.pressed() && canShoot)
    {
        setAnimationState(player_anim_state::SHOOT);
        canShoot = false;
    }
}

void PlayerSystem::onMelee(player_melee& action)
{
    if (action.pressed() && canShoot)
    {
        player.get_component<rigidbody>()->velocity = math::vec3::zero;
        setAnimationState(player_anim_state::MELEE);
        canShoot = false;
        melee = true;
    }
}

void PlayerSystem::shoot()
{
    animState = player_anim_state::SHOOT;
    auto bullet = createEntity("Bullet");
    bullet.add_component<bullet_comp>();
    position& pos = bullet.add_component<position>();
    auto& playerRot = *player.get_component<rotation>();
    auto forward = playerRot.forward();
    auto right = playerRot.right();
    pos = *player.get_component<position>() + (forward * 0.2f) + (right * -0.2f);
    bullet.add_component<rotation>();
    scale& scal = bullet.add_component<scale>();
    scal = scale(.1f);
    bullet.add_component(gfx::mesh_renderer{ gfx::MaterialCache::get_material("default"), rendering::ModelCache::get_handle("Sphere") });
    rigidbody& rb = bullet.add_component<rigidbody>();
    rb.setMass(0.1f);
    rb.addForce(forward * 100.f);
    rb.linearDrag = .1f;

    collider& col = bullet.add_component<collider>();
    col.layer = 4;
    col.ignoreMask = 1;
    col.add_shape<SphereCollider>();
}

void PlayerSystem::horizontal_move(player_horizontal& axis)
{
    movement.x = axis.value * speed;
}

void PlayerSystem::vertical_move(player_vertical& axis)
{
    movement.y = axis.value * speed;
}

