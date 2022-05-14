#include "autogen_reflector_camera_follow.hpp"
#include "../../jam/broadphase/../data/../components/camerafollowcomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::camera_follow>(::camera_follow& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::camera_follow>();
        refl.typeName = "camera_follow";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("target", member_reference("target", make_reflector(obj.target)));
        refl.members.emplace("targetOffset", member_reference("targetOffset", make_reflector(obj.targetOffset)));
        refl.members.emplace("lookAtOffset", member_reference("lookAtOffset", make_reflector(obj.lookAtOffset)));
        refl.members.emplace("lagDistance", member_reference("lagDistance", primitive_reference{typeHash(obj.lagDistance), &obj.lagDistance}));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::camera_follow>(const ::camera_follow& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::camera_follow>();
        refl.typeName = "camera_follow";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("target", member_reference("target", make_reflector(obj.target)));
        refl.members.emplace("targetOffset", member_reference("targetOffset", make_reflector(obj.targetOffset)));
        refl.members.emplace("lookAtOffset", member_reference("lookAtOffset", make_reflector(obj.lookAtOffset)));
        refl.members.emplace("lagDistance", member_reference("lagDistance", primitive_reference{typeHash(obj.lagDistance), &obj.lagDistance}));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
