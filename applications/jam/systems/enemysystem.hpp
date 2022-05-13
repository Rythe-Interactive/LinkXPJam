#pragma once
#include "../engine_include.hpp"

#include "../components/components.hpp"
#include "../data/bounding_box.hpp"

using namespace lgn;
class EnemySystem final : public legion::System<EnemySystem>
{
public:
    void setup();
    void fixedUpdate(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("EnemySystem shutdown");
    }

};
