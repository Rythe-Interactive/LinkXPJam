#pragma once
#include <core/types/prototype.hpp>
struct animated_mesh_renderer;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::animated_mesh_renderer>(const ::animated_mesh_renderer& obj);
}
