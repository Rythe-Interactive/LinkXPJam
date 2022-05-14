#include "autogen_prototype_animated_mesh_renderer.hpp"
#include "../../jam/broadphase/../data/../components/animated_mesh_renderer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::animated_mesh_renderer>(const ::animated_mesh_renderer& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::animated_mesh_renderer>();
        prot.typeName = "animated_mesh_renderer";
        return prot;
    }
}
