#include "autogen_prototype_key_frame.hpp"
#include "../../jam/broadphase/../data/../components/animated_mesh_renderer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::key_frame>(const ::key_frame& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::key_frame>();
        prot.typeName = "key_frame";
        return prot;
    }
}
