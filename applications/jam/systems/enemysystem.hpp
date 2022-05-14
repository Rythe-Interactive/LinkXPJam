#pragma once
#include "../engine_include.hpp"

#include "../components/components.hpp"
#include "../data/bounding_box.hpp"

using namespace lgn;
class EnemySystem final : public legion::System<EnemySystem>
{
public:
    ecs::filter<position, rotation, scale, rigidbody, enemy_comp> enemies;
    ecs::filter<position, rotation, scale, player_comp> players;
    ecs::entity player;
    float spawnInterval = 2.f;
    float elapsedTime = 0.0f;
    bool pause = false;

    void setup();
    void fixedUpdate(lgn::time::span);
    void spawnEnemy();
    void shutdown()
    {
        lgn::log::debug("EnemySystem shutdown");
    }

};
