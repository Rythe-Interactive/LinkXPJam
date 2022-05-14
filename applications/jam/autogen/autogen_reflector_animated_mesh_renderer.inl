#include "autogen_reflector_animated_mesh_renderer.hpp"
#include "../../jam/broadphase/../data/../components/animated_mesh_renderer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::animated_mesh_renderer>(::animated_mesh_renderer& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::animated_mesh_renderer>();
        refl.typeName = "animated_mesh_renderer";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::animated_mesh_renderer>(const ::animated_mesh_renderer& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::animated_mesh_renderer>();
        refl.typeName = "animated_mesh_renderer";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
