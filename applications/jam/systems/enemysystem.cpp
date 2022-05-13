#include "enemysystem.hpp"
#include "gui_test.hpp"
#include <rendering/debugrendering.hpp>
#include "../components/components.hpp"

void EnemySystem::setup()
{
    createProcess<&EnemySystem::fixedUpdate>("Update", 0.02f);
}

void EnemySystem::fixedUpdate(lgn::time::span dt)
{

}




