#pragma once
#include "../engine_include.hpp"

#include "../systems/gamesystem.hpp"
#include "../systems/bulletsystem.hpp"
#include "../systems/enemysystem.hpp"
#include "../systems/gui_test.hpp"
#include "../systems/physicssystem.hpp"
#include "../systems/playersystem.hpp"
#include "../systems/simplecameracontroller.hpp"
#include "../systems/killablesystem.hpp"

class GameModule : public legion::Module
{
public:
    virtual void setup() override
    {
        using namespace legion;
        app::WindowSystem::requestWindow(
            ecs::world_entity_id,
            math::ivec2(1920, 1080),
            "LEGION Engine",
            "Legion Icon",
            nullptr,
            nullptr,
            0
        );

        reportSystem<PhysicsSystem>();
        reportSystem<GameSystem>();
        reportSystem<SimpleCameraController>();
        reportSystem<BulletSystem>();
        reportSystem<EnemySystem>();
        reportSystem<PlayerSystem>();
        reportSystem<KillableSystem>();
        //reportSystem<CameraFollowSystem>();
    }
};
