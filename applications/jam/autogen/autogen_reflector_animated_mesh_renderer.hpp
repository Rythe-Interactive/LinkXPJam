#pragma once
#include <core/types/reflector.hpp>
struct animated_mesh_renderer;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::animated_mesh_renderer>(::animated_mesh_renderer& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::animated_mesh_renderer>(const ::animated_mesh_renderer& obj);
}
