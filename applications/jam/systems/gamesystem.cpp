#include "../systems/gamesystem.hpp"
#include "../components/components.hpp"
#include "../defaults/defaultpolicies.hpp"

#include <rendering/data/particlepolicies/renderpolicy.hpp>
#include <rendering/data/particlepolicies/flipbookpolicy.hpp>

#include <rendering/util/gui.hpp>
#include <rendering/pipeline/gui/stages/imguirenderstage.hpp>
#include <imgui/imgui_internal.h>

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
    }

    initInput();

    ImGuiStage::addGuiRender<GameSystem, &GameSystem::onGUI>(this);

    app::window& window = ecs::world.get_component<app::window>();
    window.enableCursor(true);
    window.show();
    app::context_guard guard(window);
    {
        auto skyboxMat = rendering::MaterialCache::create_material("skybox", "assets://shaders/skybox.shs"_view);
        skyboxMat.set_param(SV_SKYBOX, TextureCache::create_texture("planet atmo", fs::view("assets://textures/HDRI/planetatmo7.png"),
            texture_import_settings
            {
                texture_type::two_dimensional, true, channel_format::eight_bit, texture_format::rgba_hdr,
                texture_components::rgba, true, true, 0, texture_mipmap::linear_mipmap_linear, texture_mipmap::linear,
                texture_wrap::edge_clamp, texture_wrap::repeat, texture_wrap::edge_clamp
            }));
        ecs::world.add_component(gfx::skybox_renderer{ skyboxMat });

        auto groundplane = createEntity("Ground Plane");
        auto groundmat = rendering::MaterialCache::create_material("floor", "assets://shaders/groundplane.shs"_view);
        groundmat.set_param("floorTile", rendering::TextureCache::create_texture("floorTile", "engine://resources/default/tile.png"_view));
        groundplane.add_component(gfx::mesh_renderer{ groundmat, rendering::ModelCache::create_model("floor", "assets://models/plane.obj"_view) });
        groundplane.add_component<transform>();

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

            auto ent = createEntity("Sphere");
            ent.add_component(gfx::mesh_renderer{ mat, rendering::ModelCache::create_model("Sphere", "assets://models/sphere.obj"_view) });
            ent.add_component<transform>();
        }
    }



    bindToEvent<collision, &GameSystem::onCollision>();
    createProcess<&GameSystem::fixedUpdate>("Update", 0.2f);
}

void GameSystem::fixedUpdate(lgn::time::span deltaTime)
{
    //ecs::filter<position, rotation, scale, audio::audio_source> filter;
    //for (auto src_ent : filter)
    //{
    //    auto src = src_ent.get_component<audio::audio_source>();
    //    if (src->isStopped())
    //    {
    //        src_ent.destroy();
    //    }
    //}

    //for (int i = 0; i < 50; i++)
    //{
    //    auto ent = createEntity();
    //    ent.add_component<transform>();
    //    auto src = ent.add_component<audio::audio_source>(audio::AudioSegmentCache::getAudioSegment("LaserShot"));
    //    src->play();
    //}
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

    ImGui::Text("Health: %f", static_cast<float>(0));
    ImGui::End();

}

void GameSystem::spawnEnemy()
{

}


void GameSystem::onShoot(player_shoot& action)
{
    if (escaped)
        return;
}

void GameSystem::shoot(ecs::entity ship)
{
    using namespace lgn;

}

void GameSystem::onCollision(collision& event)
{
    //log::debug("Collision between {} and {}, with normal {}, and depth {}",
    //    event.first->name.empty() ? std::to_string(event.first->id) : event.first->name,
    //    event.second->name.empty() ? std::to_string(event.second->id) : event.second->name,
    //    event.normal.axis, event.normal.depth);
}

void GameSystem::initInput()
{
    using namespace legion;
    app::InputSystem::createBinding<fullscreen_action>(app::inputmap::method::F11);
    app::InputSystem::createBinding<vsync_action>(app::inputmap::method::F1);
    app::InputSystem::createBinding<tonemap_action>(app::inputmap::method::F2);
    app::InputSystem::createBinding<auto_exposure_action>(app::inputmap::method::F4);
    app::InputSystem::createBinding<exit_action>(app::inputmap::method::ESCAPE);
    app::InputSystem::createBinding<player_shoot>(app::inputmap::method::MOUSE_LEFT);

    bindToEvent<player_shoot, &GameSystem::onShoot>();

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
