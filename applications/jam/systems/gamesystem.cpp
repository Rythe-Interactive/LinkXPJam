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

        {
            audio::AudioSegmentCache::createAudioSegment("Explosion", fs::view("assets://audio/fx/Explosion2.wav"));
            audio::AudioSegmentCache::createAudioSegment("LaserShot", fs::view("assets://audio/fx/Laser_Shoot.wav"));
            audio::AudioSegmentCache::createAudioSegment("BGMusic", fs::view("assets://audio/background.mp3"));
        }

        //{
        //    gfx::MaterialCache::create_material("ShipLit", fs::view("engine://shaders/default_lit.shs"));
        //    texture_import_settings settings = gfx::default_texture_settings;
        //    settings.mag = gfx::texture_mipmap::nearest;
        //    auto color = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/ColorPalette.png"), settings);
        //    auto emissive = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/EmissivePallete.png"), settings);
        //    auto metallic = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/MetallicPalette.png"), settings);
        //    auto roughness = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/RoughnessPalette.png"), settings);
        //    gfx::MaterialCache::get_material("ShipLit").set_param("albedoTex", color);
        //    gfx::MaterialCache::get_material("ShipLit").set_param("useAlbedoTex", true);
        //    gfx::MaterialCache::get_material("ShipLit").set_param("emissiveTex", emissive);
        //    gfx::MaterialCache::get_material("ShipLit").set_param("useEmissiveTex", true);
        //    gfx::MaterialCache::get_material("ShipLit").set_param("roughnessTex", roughness);
        //    gfx::MaterialCache::get_material("ShipLit").set_param("useRoughnessTex", true);
        //    gfx::MaterialCache::get_material("ShipLit").set_param("metallicTex", metallic);
        //    gfx::MaterialCache::get_material("ShipLit").set_param("useMetallicTex", true);
        //}

        camera = createEntity("Camera");
        camera.add_component<transform>(position(0.f, 10.f, 0.f), rotation::lookat(position(0.f, 10.f, 0.f), position(0.f, 0.f, 0.f)), scale());
        rendering::camera cam;
        cam.set_projection(60.f, 0.001f, 1000.f);
        camera.add_component<gfx::camera>(cam);
    }



    bindToEvent<collision, &GameSystem::onCollision>();
    createProcess<&GameSystem::fixedUpdate>("Update", 0.02f);
}

void GameSystem::fixedUpdate(lgn::time::span deltaTime)
{

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
