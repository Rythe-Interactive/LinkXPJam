#pragma once
#include <core/core.hpp>
#include <core/math/math.hpp>

struct [[reflectable]] enemy_comp
{
    legion::core::id_type target = 0;
    float speed = 3.f;
    float initHealth = 6.f;
    bool hunt = false;
    bool running = false;
};
