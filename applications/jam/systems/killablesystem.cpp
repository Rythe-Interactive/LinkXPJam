#include "killablesystem.hpp"

void KillableSystem::setup()
{

}

void KillableSystem::fixedUpdate(lgn::time::span)
{
    for (auto ent : killables)
    {
        if (ent.get_component<killable>()->health <= 0)
            ent.destroy();
    }
}
