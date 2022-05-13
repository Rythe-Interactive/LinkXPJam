#include "autogen_prototype_stop_vel.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::stop_vel>(const ::stop_vel& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::stop_vel>();
        prot.typeName = "stop_vel";
        return prot;
    }
}
