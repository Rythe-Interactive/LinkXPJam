#include "autogen_reflector_player_horizontal.hpp"
#include "../../jam/systems/playersystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_horizontal>(::player_horizontal& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_horizontal>();
        refl.typeName = "player_horizontal";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_horizontal>(const ::player_horizontal& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_horizontal>();
        refl.typeName = "player_horizontal";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
