#include "../systems/gamesystem.hpp"
#include "../components/components.hpp"
#include "../defaults/defaultpolicies.hpp"

#include "../renderstages/animatedmeshrenderstage.hpp"

#include <rendering/data/particlepolicies/renderpolicy.hpp>
#include <rendering/data/particlepolicies/flipbookpolicy.hpp>

#include <rendering/util/gui.hpp>
#include <rendering/pipeline/gui/stages/imguirenderstage.hpp>
#include <imgui/imgui_internal.h>

lgn::ecs::entity GameSystem::core;

void GameSystem::setup()
{
    using namespace legion;
    log::filter(log::severity_debug);
    log::debug("GameSystem setup");

    auto* pipeline = dynamic_cast<gfx::DefaultPipeline*>(gfx::Renderer::getMainPipeline());
    if (pipeline)
    {
        pipeline->attachStage<ImGuiStage>();
        pipeline->attachStage<MouseHover>();
        pipeline->attachStage<AnimatedMeshRenderStage>();
    }

    initInput();

    ImGuiStage::addGuiRender<GameSystem, &GameSystem::onGUI>(this);

    app::window& window = ecs::world.get_component<app::window>();
    window.enableCursor(true);
    window.show();
    app::context_guard guard(window);
    {
        auto skyboxMat = rendering::MaterialCache::create_material("skybox", "assets://shaders/skybox.shs"_view);
        skyboxMat.set_param(SV_SKYBOX, TextureCache::create_texture("planet atmo", fs::view("assets://textures/HDRI/park.jpg"),
            texture_import_settings
            {
                texture_type::two_dimensional, true, channel_format::eight_bit, texture_format::rgba_hdr,
                texture_components::rgba, true, true, 0, texture_mipmap::linear_mipmap_linear, texture_mipmap::linear,
                texture_wrap::edge_clamp, texture_wrap::repeat, texture_wrap::edge_clamp
            }));
        ecs::world.add_component(gfx::skybox_renderer{ skyboxMat });

        auto groundplane = createEntity("Ground Plane");
        auto groundmat = rendering::MaterialCache::create_material("floor", "assets://shaders/texture.shs"_view);
        groundmat.set_param("_texture", rendering::TextureCache::create_texture("floorTile", "assets://textures/big_room_red.png"_view));
        groundplane.add_component(gfx::mesh_renderer{ groundmat, rendering::ModelCache::create_model("floor", "assets://models/plane.obj"_view) });
        groundplane.add_component<transform>();
        groundplane.get_component<position>() = position(0.f, -2.f, 0.f);
        groundplane.get_component<scale>() = scale(30.f, 1.f, 20.f);

        {
            audio::AudioSegmentCache::createAudioSegment("Explosion", fs::view("assets://audio/fx/Explosion2.wav"));
            audio::AudioSegmentCache::createAudioSegment("LaserShot", fs::view("assets://audio/fx/Laser_Shoot.wav"));
            audio::AudioSegmentCache::createAudioSegment("BGMusic", fs::view("assets://audio/background.mp3"));
        }

        {
            auto mat = gfx::MaterialCache::create_material("default", fs::view("assets://shaders/pbr.shs"));
            auto color = gfx::TextureCache::create_texture(fs::view("engine://resources/default/albedo.png"));
            auto emissive = gfx::TextureCache::create_texture(fs::view("engine://resources/default/emissive.png"));
            auto mrdao = gfx::TextureCache::create_texture(fs::view("engine://resources/default/MRDAo.png"));
            auto normalHeight = gfx::TextureCache::create_texture(fs::view("engine://resources/default/normalHeight.png"));
            mat.set_param(SV_ALBEDO, color);
            mat.set_param(SV_EMISSIVE, emissive);
            mat.set_param(SV_MRDAO, mrdao);
            mat.set_param(SV_NORMALHEIGHT, normalHeight);
            mat.set_param(SV_HEIGHTSCALE, 1.f);
        }

        {
            auto mat = gfx::MaterialCache::create_material("playerMat", fs::view("engine://shaders/default_lit.shs"));
            auto color = gfx::TextureCache::create_texture(fs::view("assets://textures/player/Base_color.png"));
            auto height = gfx::TextureCache::create_texture(fs::view("assets://textures/player/Height.png"));
            auto metallic = gfx::TextureCache::create_texture(fs::view("assets://textures/player/Metallic.png"));
            auto roughness = gfx::TextureCache::create_texture(fs::view("assets://textures/player/Roughness.png"));
            auto normal = gfx::TextureCache::create_texture(fs::view("assets://textures/player/Normal.png"));
            mat.set_param("useAlbedoTex", true);
            mat.set_param("albedoTex", color);
            mat.set_param("useMetallicTex", true);
            mat.set_param("metallicTex", metallic);
            mat.set_param("useRoughnessTex", true);
            mat.set_param("roughnessTex", roughness);
            mat.set_param("useNormal", true);
            mat.set_param("normalTex", normal);
            mat.set_param("useHeight", true);
            mat.set_param("heightTex", height);
        }
    }

    auto camera = createEntity("Camera");
    camera.add_component<transform>(position(0.f, 20.f, 0.f), rotation::lookat(position(0.f, 20.f, 0.f), math::vec3(0.f, 0.f, 0.0001f), math::vec3::up), scale());
    camera.add_component<audio::audio_listener>();

    rendering::camera cam;
    cam.set_projection(60.f, 0.001f, 1000.f);
    camera.add_component<gfx::camera>(cam);

    core = createEntity();
    position& pos = core.add_component<position>();
    core.add_component<rotation>();
    core.add_component<scale>();
    auto& rb = *core.add_component<rigidbody>();
    rb.setMass(1000.f);
    rb.linearDrag = 0.9f;
    killable& k = core.add_component<killable>();
    k.health = 100.f;
    core.add_component(gfx::mesh_renderer{ gfx::MaterialCache::get_material("default"), rendering::ModelCache::create_model("Sphere", "assets://models/sphere.obj"_view) });

    collider& col = core.add_component<collider>();
    col.layer = 0;
    col.ignoreMask = 0;
    col.add_shape<SphereCollider>();

    bindToEvent<collision, &GameSystem::onCollision>();
}

void GameSystem::update(time::span dt)
{
    deltaTime = dt;
}

void GameSystem::onGUI(app::window& context, L_MAYBEUNUSED gfx::camera& cam, L_MAYBEUNUSED const gfx::camera::camera_input& camInput, time::span deltaTime)
{
    using namespace imgui;

    auto windowSize = context.framebufferSize();

    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));

    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiColorEditFlags_NoInputs | ImGuiWindowFlags_NoMouseInputs);

    auto* imwindow = ImGui::GetCurrentWindow();
    imwindow->FontWindowScale = 2.f;

    ImGui::Text("SCORE: %d", static_cast<int>(69));
    ImGui::End();


    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowPos({ 0,10.f });
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiColorEditFlags_NoInputs | ImGuiWindowFlags_NoMouseInputs);

    imwindow = ImGui::GetCurrentWindow();
    imwindow->FontWindowScale = 2.f;

    if (!core)
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Health: %f", core.get_component<killable>()->health);
    ImGui::End();

}


void GameSystem::onCollision(collision& event)
{
    //log::debug("Collision between {} and {}, with normal {}, and depth {}",
    //    event.first->name.empty() ? std::to_string(event.first->id) : event.first->name,
    //    event.second->name.empty() ? std::to_string(event.second->id) : event.second->name,
    //    event.normal.axis, event.normal.depth);

    ecs::entity first = event.first;
    ecs::entity second = event.second;

    rigidbody& firstRB = first.get_component<rigidbody>();
    rigidbody& secondRB = second.get_component<rigidbody>();

    if (first.has_component<killable>())
    {
        if (second.has_component<bullet_comp>())
        {
            auto dmg = second.get_component<bullet_comp>()->damge;
            first.get_component<killable>()->health -= dmg;
        }
    }

    if (second.has_component<killable>())
    {
        if (first.has_component<bullet_comp>())
        {
            auto dmg = first.get_component<bullet_comp>()->damge;
            second.get_component<killable>()->health -= dmg;
        }
    }

    if (first == core)
    {
        if (second.has_component<enemy_comp>())
        {
            core.get_component<killable>()->health -= deltaTime * 2.f;
        }
    }
    else if (second == core)
    {
        if (first.has_component<enemy_comp>())
        {
            core.get_component<killable>()->health -= deltaTime * 2.f;
        }
    }

    float firstMass = 1.f / firstRB.inverseMass;
    float secondMass = 1.f / secondRB.inverseMass;

    float firstImpactForce = math::abs(math::dot(firstRB.velocity, event.normal.axis));
    float secondImpactForce = math::abs(math::dot(secondRB.velocity, event.normal.axis));
    float force = firstImpactForce + secondImpactForce;

    float totalMass = firstMass + secondMass;
    float firstImpactFract = secondMass / totalMass;
    float secondImpactFract = firstMass / totalMass;

    *first.get_component<position>() += -event.normal.axis * event.normal.depth * firstImpactFract;
    *second.get_component<position>() += event.normal.axis * event.normal.depth * secondImpactFract;

    math::vec3 acc = -event.normal.axis * force * firstImpactFract * (1.f + firstRB.restitution);
    firstRB.velocity += acc;
    acc = event.normal.axis * force * secondImpactFract * (1.f + secondRB.restitution);
    secondRB.velocity += acc;

    if (second.has_component<bullet_comp>())
        second.remove_component<collider>();
    if (first.has_component<bullet_comp>())
        first.remove_component<collider>();

}

void GameSystem::initInput()
{
    using namespace legion;
    app::InputSystem::createBinding<fullscreen_action>(app::inputmap::method::F11);
    app::InputSystem::createBinding<vsync_action>(app::inputmap::method::F1);
    app::InputSystem::createBinding<tonemap_action>(app::inputmap::method::F2);
    app::InputSystem::createBinding<auto_exposure_action>(app::inputmap::method::F4);
    app::InputSystem::createBinding<exit_action>(app::inputmap::method::ESCAPE);
    //app::InputSystem::createBinding<player_shoot>(app::inputmap::method::MOUSE_LEFT);

    //bindToEvent<player_shoot, &GameSystem::onShoot>();

    bindToEvent<tonemap_action, &GameSystem::onTonemapSwitch>();
    bindToEvent<auto_exposure_action, &GameSystem::onAutoExposureSwitch>();
    bindToEvent<exit_action, &GameSystem::onExit>();
    bindToEvent<fullscreen_action, &GameSystem::onFullscreen>();
    bindToEvent<vsync_action, &GameSystem::onVSYNCSwap>();
}

void GameSystem::onAutoExposureSwitch(auto_exposure_action& event)
{
    using namespace legion;
    if (event.released())
    {
        static float defaultExposure = gfx::Tonemapping::getExposure();
        bool enabled = !gfx::Tonemapping::autoExposureEnabled();
        gfx::Tonemapping::enableAutoExposure(enabled);
        if (!enabled)
            gfx::Tonemapping::setExposure(defaultExposure);

        log::debug("Auto exposure {}", enabled ? "enabled" : "disabled");
    }
}
void GameSystem::onTonemapSwitch(tonemap_action& event)
{
    using namespace legion;
    if (event.released())
    {
        static size_type type = static_cast<size_type>(gfx::tonemapping_type::aces);
        type = (type + 1) % (static_cast<size_type>(gfx::tonemapping_type::unreal3) + 1);

        auto typeEnum = static_cast<gfx::tonemapping_type>(type);

        std::string algorithmName;
        switch (typeEnum)
        {
        case gfx::tonemapping_type::aces:
            algorithmName = "aces";
            break;
        case gfx::tonemapping_type::reinhard:
            algorithmName = "reinhard";
            break;
        case gfx::tonemapping_type::reinhard_jodie:
            algorithmName = "reinhard_jodie";
            break;
        case gfx::tonemapping_type::legion:
            algorithmName = "legion";
            break;
        case gfx::tonemapping_type::unreal3:
            algorithmName = "unreal3";
            break;
        default:
            algorithmName = "legion";
            break;
        }
        log::debug("Set tonemapping algorithm to {}", algorithmName);

        gfx::Tonemapping::setAlgorithm(typeEnum);
    }
}
void GameSystem::onExit(exit_action& action)
{
    using namespace lgn;
    if (GuiTestSystem::isEditingText)
        return;

    if (action.released())
        raiseEvent<events::exit>();
}
void GameSystem::onFullscreen(fullscreen_action& action)
{
    using namespace lgn;
    if (GuiTestSystem::isEditingText)
        return;

    if (action.released())
    {
        app::WindowSystem::requestFullscreenToggle(ecs::world_entity_id, math::ivec2(100, 100), math::ivec2(1360, 768));
    }
}
void GameSystem::onVSYNCSwap(vsync_action& action)
{
    using namespace lgn;
    if (GuiTestSystem::isEditingText)
        return;

    if (action.released())
    {
        app::window& window = ecs::world.get_component<app::window>();
        window.setSwapInterval(window.swapInterval() ? 0 : 1);
        log::debug("set swap interval to {}", window.swapInterval());
    }
}
