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

    app::InputSystem::createBinding<player_horizontal>(app::inputmap::method::A, -1);
    app::InputSystem::createBinding<player_horizontal>(app::inputmap::method::D, 1);
    app::InputSystem::createBinding<player_vertical>(app::inputmap::method::W, 1);
    app::InputSystem::createBinding<player_vertical>(app::inputmap::method::S, -1);

    bindToEvent<player_horizontal, &PlayerSystem::horizontal_move>();
    bindToEvent<player_vertical, &PlayerSystem::vertical_move>();

    createProcess<&PlayerSystem::fixedUpdate>("Update", 0.02f);
}


void PlayerSystem::fixedUpdate(lgn::time::span deltaTime)
{

}

void PlayerSystem::horizontal_move(player_horizontal& axis)
{

}

void PlayerSystem::vertical_move(player_vertical& axis)
{

}
