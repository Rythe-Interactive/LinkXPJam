#include "../systems/playersystem.hpp"
#include "../debug_utils.hpp"

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
    createProcess<&PlayerSystem::fixedUpdate>("Update", 0.02f);
}


void PlayerSystem::fixedUpdate(lgn::time::span deltaTime)
{
    
}
