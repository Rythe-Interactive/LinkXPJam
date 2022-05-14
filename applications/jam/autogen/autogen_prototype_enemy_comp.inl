#include "autogen_prototype_enemy_comp.hpp"
#include "../../jam/components/enemycomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::enemy_comp>(const ::enemy_comp& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::enemy_comp>();
        prot.typeName = "enemy_comp";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("target", member_value("target", primitive_value{typeHash(obj.target), std::any(obj.target)}));
        prot.members.emplace("speed", member_value("speed", primitive_value{typeHash(obj.speed), std::any(obj.speed)}));
        prot.members.emplace("initHealth", member_value("initHealth", primitive_value{typeHash(obj.initHealth), std::any(obj.initHealth)}));
        prot.members.emplace("hunt", member_value("hunt", primitive_value{typeHash(obj.hunt), std::any(obj.hunt)}));
        prot.members.emplace("running", member_value("running", primitive_value{typeHash(obj.running), std::any(obj.running)}));
        return prot;
    }
}
