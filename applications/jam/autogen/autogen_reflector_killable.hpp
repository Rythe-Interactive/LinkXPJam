#pragma once
#include <core/types/reflector.hpp>
struct killable;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::killable>(::killable& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::killable>(const ::killable& obj);
}
