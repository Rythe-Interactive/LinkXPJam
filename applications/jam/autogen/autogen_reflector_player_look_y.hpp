#pragma once
#include <core/types/reflector.hpp>
struct player_look_y;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_look_y>(::player_look_y& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_look_y>(const ::player_look_y& obj);
}
