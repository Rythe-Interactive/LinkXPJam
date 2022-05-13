#pragma once
#include <core/types/prototype.hpp>
struct stop_vel;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::stop_vel>(const ::stop_vel& obj);
}
