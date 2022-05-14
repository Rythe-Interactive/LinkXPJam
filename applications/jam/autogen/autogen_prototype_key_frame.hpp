#pragma once
#include <core/types/prototype.hpp>
struct key_frame;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::key_frame>(const ::key_frame& obj);
}
