#include "autogen_prototype_player_horizontal.hpp"
#include "../../jam/systems/playersystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_horizontal>(const ::player_horizontal& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_horizontal>();
        prot.typeName = "player_horizontal";
        return prot;
    }
}
