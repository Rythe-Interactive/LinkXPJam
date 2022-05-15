#include "killablesystem.hpp"

#include "gamesystem.hpp"

void KillableSystem::setup()
{
    createProcess<&KillableSystem::fixedUpdate>("Update", 0.2f);
}

void KillableSystem::fixedUpdate(lgn::time::span)
{
    for (auto ent : killables)
    {
        auto health = ent.get_component<killable>()->health;

        if (health <= 0)
        {
            if (ent == GameSystem::core)
            {
                raiseEvent<events::exit>();
                continue;
            }
            ent.destroy();
        }
    }
}
