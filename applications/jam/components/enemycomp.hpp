#pragma once
#include <core/core.hpp>
#include <core/math/math.hpp>

struct [[reflectable]] enemy_comp
{
    legion::core::id_type target = 0;
    float speed = 2.f;
    float initHealth = 20.0f;
    bool hunt = false;
    bool running = false;
};
