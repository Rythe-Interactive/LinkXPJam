#pragma once
#include <core/types/prototype.hpp>
struct killable;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::killable>(const ::killable& obj);
}
