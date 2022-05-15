#include "enemysystem.hpp"
#include "gui_test.hpp"
#include <rendering/debugrendering.hpp>
#include "../components/components.hpp"
#include "gamesystem.hpp"

void EnemySystem::setup()
{

    createProcess<&EnemySystem::fixedUpdate>("Update", 0.02f);
}

void EnemySystem::fixedUpdate(lgn::time::span dt)
{
    for (auto& ent : players)
    {
        player = ent;
    }

    if (!player)
        return;

    if (!pause)
    {
        elapsedTime += static_cast<float>(dt);
        if (elapsedTime > spawnInterval)
        {
            elapsedTime = 0.0f;
            spawnEnemy();
        }
    }

    position& playerPos = player.get_component<position>();
    auto& corePos = *GameSystem::core.get_component<position>();
    for (auto& ent : enemies)
    {
        enemy_comp enemyComp = ent.get_component<enemy_comp>();
        position& pos = ent.get_component<position>();

        velocity sumVel = math::vec3::zero;;

        for (auto other : enemies)
        {
            if (other->id == ent->id)
                continue;

            position& otherPos = other.get_component<position>();

            if (math::distance2(pos, static_cast<math::vec3>(otherPos)) < 2.f)
            {
                sumVel += (pos - otherPos);
            }
        }

        if (math::length(static_cast<math::vec3>(sumVel)) > 0.0f)
            sumVel = math::normalize(sumVel) * enemyComp.speed;

        float playerDist = math::length2(playerPos - pos);
        float coreDist = math::length2(corePos - pos);
        if (playerDist < coreDist)
            sumVel += math::normalize(playerPos - pos) * enemyComp.speed;
        else
            sumVel += math::normalize(corePos - pos) * enemyComp.speed;

        if (math::length(static_cast<math::vec3>(sumVel)) > 0.0f)
        {
            rigidbody& rb = ent.get_component<rigidbody>();
            rb.velocity = sumVel;
        }
    }
}

void EnemySystem::spawnEnemy()
{
    auto ent = createEntity("Enemy");
    position& pos = ent.add_component<position>();
    auto randomPos = math::circularRand(10.f);
    pos = math::vec3(randomPos.x, 0.f, randomPos.y);

    ent.add_component<rotation>();
    ent.add_component<scale>();
    enemy_comp& enemyComp = ent.add_component<enemy_comp>();
    killable& k = ent.add_component<killable>();
    k.health = enemyComp.initHealth;

    ent.add_component<rigidbody>();

    collider& col = ent.add_component<collider>();
    col.add_shape<SphereCollider>();
    col.layer = 2;

    ent.add_component(animated_mesh_renderer(gfx::MaterialCache::get_material("default"), key_frame_list{
    { gfx::ModelCache::create_model("enemy0", "assets://models/enemy/enemy0.glb"_view), 0.25f },
    { gfx::ModelCache::create_model("enemy1", "assets://models/enemy/enemy1.glb"_view), 0.25f },
        }));

}




