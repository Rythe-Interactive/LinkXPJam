#pragma once
#include <core/types/reflector.hpp>
struct player_horizontal;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_horizontal>(::player_horizontal& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_horizontal>(const ::player_horizontal& obj);
}
