#include "autogen_prototype_camera_follow.hpp"
#include "../../jam/broadphase/../data/../components/camerafollowcomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::camera_follow>(const ::camera_follow& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::camera_follow>();
        prot.typeName = "camera_follow";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("target", member_value("target", make_prototype(obj.target)));
        prot.members.emplace("targetOffset", member_value("targetOffset", make_prototype(obj.targetOffset)));
        prot.members.emplace("lookAtOffset", member_value("lookAtOffset", make_prototype(obj.lookAtOffset)));
        prot.members.emplace("lagDistance", member_value("lagDistance", primitive_value{typeHash(obj.lagDistance), std::any(obj.lagDistance)}));
        return prot;
    }
}
