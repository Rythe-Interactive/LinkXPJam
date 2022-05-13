#pragma once
#include <core/types/reflector.hpp>
struct stop_vel;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::stop_vel>(::stop_vel& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::stop_vel>(const ::stop_vel& obj);
}
