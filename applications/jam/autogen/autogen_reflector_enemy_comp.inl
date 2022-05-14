#include "autogen_reflector_enemy_comp.hpp"
#include "../../jam/components/enemycomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::enemy_comp>(::enemy_comp& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::enemy_comp>();
        refl.typeName = "enemy_comp";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("target", member_reference("target", primitive_reference{typeHash(obj.target), &obj.target}));
        refl.members.emplace("health", member_reference("health", primitive_reference{typeHash(obj.health), &obj.health}));
        refl.members.emplace("speed", member_reference("speed", primitive_reference{typeHash(obj.speed), &obj.speed}));
        refl.members.emplace("hunt", member_reference("hunt", primitive_reference{typeHash(obj.hunt), &obj.hunt}));
        refl.members.emplace("running", member_reference("running", primitive_reference{typeHash(obj.running), &obj.running}));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::enemy_comp>(const ::enemy_comp& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::enemy_comp>();
        refl.typeName = "enemy_comp";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("target", member_reference("target", primitive_reference{typeHash(obj.target), &obj.target}));
        refl.members.emplace("health", member_reference("health", primitive_reference{typeHash(obj.health), &obj.health}));
        refl.members.emplace("speed", member_reference("speed", primitive_reference{typeHash(obj.speed), &obj.speed}));
        refl.members.emplace("hunt", member_reference("hunt", primitive_reference{typeHash(obj.hunt), &obj.hunt}));
        refl.members.emplace("running", member_reference("running", primitive_reference{typeHash(obj.running), &obj.running}));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
