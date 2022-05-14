#include "autogen_reflector_stop_vel.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::stop_vel>(::stop_vel& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::stop_vel>();
        refl.typeName = "stop_vel";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::stop_vel>(const ::stop_vel& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::stop_vel>();
        refl.typeName = "stop_vel";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
