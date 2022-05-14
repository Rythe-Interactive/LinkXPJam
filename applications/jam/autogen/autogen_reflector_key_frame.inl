#include "autogen_reflector_key_frame.hpp"
#include "../../jam/broadphase/../data/../components/animated_mesh_renderer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::key_frame>(::key_frame& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::key_frame>();
        refl.typeName = "key_frame";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::key_frame>(const ::key_frame& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::key_frame>();
        refl.typeName = "key_frame";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
