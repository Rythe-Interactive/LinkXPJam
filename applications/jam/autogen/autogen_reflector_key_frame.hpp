#pragma once
#include <core/types/reflector.hpp>
struct key_frame;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::key_frame>(::key_frame& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::key_frame>(const ::key_frame& obj);
}
