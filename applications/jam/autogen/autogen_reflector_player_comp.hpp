#pragma once
#include <core/types/reflector.hpp>
struct player_comp;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_comp>(::player_comp& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_comp>(const ::player_comp& obj);
}
